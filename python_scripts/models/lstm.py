import numpy as np
import tensorflow as tf

from .attention import SimpleAttentionMechanism
from .imodel import IModel


class LSTM(IModel):
	def __init__(self,num_commands, num_inputs, num_outs, max_prog_length,
				embeddings_size, learning_rate=1e-3):
		IModel.__init__(self,num_commands, num_inputs, num_outs, learning_rate)
		
		self.max_prog_length = max_prog_length
		self.embeddings_size = embeddings_size

		self._build_placeholders()
		self._build_model()
		self._build_output()
		self._build_loss()
		self._build_optimizer()

		self._init = tf.global_variables_initializer()

	def _build_placeholders(self):
		self.input_command = tf.placeholder(tf.int32, [None,self.max_prog_length])
		self.input_arg = tf.placeholder(tf.float32, [None,self.max_prog_length])
		self.input = tf.placeholder(tf.float32, [None, self.num_inputs])
		self.input_len = tf.placeholder(tf.float32, [None])
		self.target = tf.placeholder(tf.float32, [None, self.num_outs])
		self._lr_ph = tf.placeholder(tf.float32, [])

	def _build_model(self):	

		embedding = tf.get_variable("embedding", [self.num_commands, self.embeddings_size], dtype=tf.float32)
		net_input_c = tf.nn.embedding_lookup(embedding, self.input_command) # B x T x embedding_size
		net_input_a = tf.reshape(self.input_arg, [-1,self.max_prog_length,1]) # B x T x 1
		net_input_inp = tf.reshape(self.input, [-1,1,self.num_inputs])
		net_input_inp = tf.tile(net_input_inp,[1,self.max_prog_length,1])
		net_input = tf.concat([net_input_c,net_input_a,net_input_inp], 2) # B x T x (num_inputs+embedding_size+1)

		net = tf.contrib.layers.fully_connected(net_input, self.num_inputs+self.embeddings_size+1, activation_fn = tf.nn.relu)
		lstm_cell = tf.contrib.rnn.MultiRNNCell([tf.contrib.rnn.BasicLSTMCell(self.num_inputs*2) for _ in range(2)])

		output_value, _ = tf.nn.dynamic_rnn (lstm_cell, net, time_major=False,dtype=tf.float32)
		attention = SimpleAttentionMechanism(self.num_outs*2)
		net, _ = attention(output_value)

		self.out = tf.contrib.layers.fully_connected(net, self.num_outs*2, activation_fn = tf.nn.relu)

	def _build_output(self):
		self.out = tf.contrib.layers.fully_connected(self.out, self.num_outs, activation_fn = tf.nn.relu)

	def _build_loss(self):
		self.loss = tf.reduce_mean(tf.abs(self.out - self.target))

	def _build_optimizer(self):
		self.learning_rate = tf.Variable(self.learning_rate)
		self._set_learning_rate = self.learning_rate.assign(self._lr_ph)
		self.global_step = tf.Variable(0,trainable=False)

		self.opt = tf.train.AdamOptimizer(self.learning_rate).minimize(self.loss,global_step=self.global_step)

	def initialize(self,sess):
		self.sess = sess
		self.sess.run(self._init)

	def set_learning_rate(self, LR):
		self.sess.run(self._set_learning_rate, feed_dict={ self._lr_ph: LR})
	def get_learning_rate(self):
		return self.sess.run(self.learning_rate)
	def get_global_step(self):
		return self.sess.run(self.global_step)

	def predict(self, X_command,X_arg,X_input,lens):
		return self.sess.run(self.out, feed_dict={ self.input_command: X_command,
													self.input_arg: X_arg,
													self.input: X_input,
													self.input_len : lens })

	def get_loss(self, X_command,X_arg,X_input,lens,Y):
		return self.sess.run(self.loss, feed_dict={ self.input_command: X_command,
													self.input_arg: X_arg,
													self.input: X_input,
													self.input_len : lens,
												self.target: np.reshape(Y,(-1,self.num_outs)) })

	def train_step(self, X_command,X_arg,X_input,lens,Y):
		return self.sess.run(self.opt, feed_dict={ self.input_command: X_command,
													self.input_arg: X_arg,
													self.input: X_input,
													self.input_len : lens,
												self.target: np.reshape(Y,(-1,self.num_outs)) })