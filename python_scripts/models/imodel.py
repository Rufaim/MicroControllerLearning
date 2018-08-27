class IModel(object):
	def __init__(self,num_commands, num_inputs, num_outs, learning_rate):
		self.num_commands = num_commands
		self.num_inputs = num_inputs
		self.num_outs = num_outs
		self.learning_rate = learning_rate

	def initialize(self,sess):
		raise NotImplementedError
	def set_learning_rate(self, LR):
		raise NotImplementedError
	def get_learning_rate(self):
		return self.sess.run(self.learning_rate)
	def predict(self, X_command, X_arg, X_input):
		raise NotImplementedError
	def get_loss(self, X_command, X_arg, X_input, Y):
		raise NotImplementedError
	def train_step(self, X_command, X_arg, X_input, Y):
		raise NotImplementedError