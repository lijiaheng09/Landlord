files = [
	'card.h',
	'card.hpp',
	'main.h',
	'card.cpp',
	'ljh.cpp',
	'cwy.cpp',
	'wzp.cpp',
	'main.cpp'
]

if __name__ == '__main__':
	res = []
	for f in files:
		res.append('// ' + f + '\n')
		with open(f, 'r') as fp:
			res.extend(fp.readlines())
		res.append('\n')
	ress = []
	for line in res:
		if line.startswith('#include') and any([f in line for f in files]):
			line = '// ' + line
		ress.append(line)
	with open('res.cpp', 'w') as fp:
		fp.writelines(ress)
