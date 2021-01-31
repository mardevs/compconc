class Buffer:
	def __init__(self, tamanho):
		self.tamanho = tamanho
		self.elementos = 0
	
	def insere(self):
		if self.cheio():
			print("ERRO: Inseriu quando buffer estava cheio")
		self.elementos += 1
	
	def remove(self):
		if self.vazio():
			print("ERRO: Removeu quando buffer estava vazio")
		self.elementos -= 1

	def cheio(self):
		return self.elementos >= self.tamanho
	
	def vazio(self):
		return self.elementos <= 0

class LE:
	def __init__(self, buffer):
		self.buffer = buffer;

	def leitorBloqueado(self, id):
		'''Recebe o id do leitor. Verifica se a decisao de bloqueio esta correta.'''
		if not self.buffer.vazio():
			print("ERRO: leitor " + str(id) + " bloqueado quando buffer nao esta vazio!")

	def escritorBloqueado(self, id):
		'''Recebe o id do escritor. Verifica se a decisao de bloqueio esta correta.'''
		if not self.buffer.cheio():
			print("ERRO: escritor " + str(id) + " bloqueado quando buffer nao esta cheio!")

	def leitorLendo(self, id):
		'''Recebe o id do leitor, verifica se pode ler e registra que esta lendo.'''
		if self.buffer.vazio():
			print("ERRO: leitor " + str(id) + " esta lendo quando buffer esta vazio!")

	def escritorEscrevendo(self, id):
		'''Recebe o id do escritor, verifica se pode escrever e registra que esta escrevendo'''
		if self.buffer.cheio():
			print("ERRO: escritor " + str(id) + " esta escrevendo quando buffer esta cheio!")
