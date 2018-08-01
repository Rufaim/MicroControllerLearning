import tensorflow as tf



class SimpleAttentionMechanism(object):
	def __init__(self, attention_size, w_init=tf.truncated_normal_initializer(stddev=0.02),
				b_init=tf.constant_initializer(0.),u_init=tf.truncated_normal_initializer(stddev=0.02)):
		assert attention_size > 0

		self.attention_size = attention_size
		self.w_init = w_init
		self.b_init = b_init
		self.u_init = u_init

	def __call__(self, input):
		TShape = input.shape

		W_ = tf.Variable(self.w_init([TShape.as_list()[2], self.attention_size]))
		b_ = tf.Variable(self.b_init([1,self.attention_size]))
		u_ = tf.Variable(self.u_init([self.attention_size]))

		# Applying fully connected layer with non-linear activation to each of the B*T timestamps;
		#  the shape of `v` is (B,T,D)*(D,A)=(B,T,A), where A=attention_size
		v = tf.nn.tanh(tf.tensordot(input, W_, axes=1) + b_)
		# For each of the timestamps its vector of size A from `v` is reduced with `u` vector
		vu = tf.tensordot(v, u_, axes=1)  # (B,T) shape
		alpha = tf.nn.softmax(vu)         # (B,T) shape also

		# Output of (Bi-)RNN is reduced with attention vector; the result has (B,D) shape
		output = tf.reduce_sum(input * tf.expand_dims(alpha, -1), 1)
		output.set_shape([None,TShape.as_list()[2]])
		return output, [W_,b_,u_]
