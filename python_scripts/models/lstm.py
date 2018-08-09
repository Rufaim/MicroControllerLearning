import numpy as np
import tensorflow as tf

from .attention import SimpleAttentionMechanism
from .imodel import IModel


class LSTM(IModel):
	def __init__(self,num_inputs, num_outs, learning_rate):
		IModel.__init__(self,num_inputs, num_outs, learning_rate)

		self._build_placeholders()
		self._build_model()
		self._build_output()
		self._build_loss()
		self._build_optimizer()

		self._init = tf.global_variables_initializer()

	def _build_placeholders(self):
		self.input = tf.placeholder(tf.float32, [None, self.num_inputs])
		self.target = tf.placeholder(tf.float32, [None, self.num_outs])
		self._lr_ph = tf.placeholder(tf.float32, [])

	def _build_model(self):	
		net = tf.contrib.layers.fully_connected(self.input, self.num_inputs*2, activation_fn = tf.relu)
		lstm_cell = tf.contrib.rnn.MultiRNNCell([tf.contrib.rnn.BasicLSTMCell(self.num_inputs*2) for _ in range(2)])

		output_value, _ = tf.nn.dynamic_rnn (lstm_cell, net, time_major=False)
		attention = SimpleAttentionMechanism(self.num_outs*2)
		net = attention(attention)

		self.out = tf.contrib.layers.fully_connected(net, self.num_outs*2, activation_fn = tf.relu)

	def _build_output(self):
		self.out = tf.contrib.layers.fully_connected(self.out, self.num_outs, activation_fn = tf.relu)

	def _build_loss(self):
		self.loss = tf.reduce_mean(tf.abs(self.out - self.target))

	def _build_optimizer(self):
		self.learning_rate = tf.Variable(self.learning_rate)
		self._set_learning_rate = self.learning_rate.assign(self._lr_ph)

		self.opt = tf.train.AdamOptimizer(self.learning_rate).minimize(self.loss)

	def initialize(self,sess):
		self.sess = sess
		self.sess.run(self._init)

	def set_learning_rate(self, LR):
		self.sess.run(self._set_learning_rate, feed_dict={ self._lr_ph: LR})
	def get_learning_rate(self):
		return self.sess.run(self.learning_rate)

	def predict(self, X):
		return self.sess.run(self.out, feed_dict={ self.input: np.reshape(X,(-1,self.num_inputs)) })

	def get_loss(self, X, Y):
		return self.sess.run(self.loss, feed_dict={ self.input: np.reshape(X,(-1,self.num_inputs)),
												self.target: np.reshape(Y,(-1,self.num_outs)) })

	def train_step(self, X, Y):
		return self.sess.run(self.opt, feed_dict={ self.input: np.reshape(X,(-1,self.num_inputs)),
												self.target: np.reshape(Y,(-1,self.num_outs)) })