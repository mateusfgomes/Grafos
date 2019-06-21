import nltk 
from nltk.stem import RSLPStemmer



def Tokenize(sentence):
    sentence = sentence.lower()
    sentence = nltk.word_tokenize(sentence)
    return sentence




def Stemming(sentence):
    stemmer = RSLPStemmer()
    phrase = [ ]
    for word in sentence:
        phrase.append(stemmer.stem(word.lower()))
    return phrase


def RemoveStopWords(sentence):
    stopwords = nltk.corpus.stopwords.words('portuguese')
    phrase = []
    for word in sentence:
        if word not in stopwords:
            if word != "sinopse" and word != ":" and word != "," and word != "." and word != "(" and word != ")":
            	phrase.append(word)
    return phrase


file = open("sinopses.txt", 'r')
output = open("saida.txt", 'w')

line = file.readline()
i = 0


while line:

	text = line.split(':')
	text = text[0]
	if text == "nome": #Caso entre aqui, novo filme
		if i != 0:
			output.write("\n\n")
		output.write(line)
		line = file.readline()
		output.write(line)
		line = file.readline()
		output.write(line)
		line = file.readline()
		output.write("sinopse:")
		i = i + 1
		
	
	line = Tokenize(line)
	line = RemoveStopWords(line)
	line = Stemming(line)
	for word in line:
		output.write(word)
		output.write(" ")

	output.write("\n")	

	line = file.readline()
	



file.close()
output.close()
