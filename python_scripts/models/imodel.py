class IModel(object):
	def __init__(self,num_inputs, num_outs, learning_rate):
		self.num_inputs = num_inputs
		self.num_outs = num_outs
		self.learning_rate = learning_rate

	def initialize(self,sess):
		raise NotImplementedError
	def set_learning_rate(self, LR):
		raise NotImplementedError
	def get_learning_rate(self):
		return self.sess.run(self.learning_rate)
	def predict(self, X):
		raise NotImplementedError
	def get_loss(self, X, Y):
		raise NotImplementedError
	def train_step(self, X, Y):
		raise NotImplementedError