import random
import os
import r2pipe

os.system("cp license_2 license_2_fuzz")

def flip_byte(in_bytes):
	i = random.randint(0,len(in_bytes))
	c = chr(random.randint(0,0xFF))
	return in_bytes[:i]+c+in_bytes[i+1:]

def copy_binary():
	with open("license_2", "rb") as orig_f, open("license_2_fuzz", "wb") as new_f:
		new_f.write(flip_byte(orig_f.read()))

def compare(fn1, fn2):
	with open(fn1) as f1, open(fn2) as f2:
		return f1.read()==f2.read()

def check_output():
	os.system("(./license_2_fuzz ; ./license_2_fuzz AAAA-Z10N-42-OK) > fuzz_output")
	return compare("orig_output", "fuzz_output")

def check_gdb():
	os.system("gdb ./license_2_fuzz -batch -ex 'disassemble main' > fuzz_gdb")
	return compare("orig_gdb", "fuzz_gdb")

def check_radare():
	r = r2pipe.open('license_2_fuzz')
	with open("fuzz_radare", "w") as fr:
		fr.write(r.cmd('aaa;s sym.main;pdf'))
	return compare("orig_radare", "fuzz_radare")

while True:
	copy_binary()
	if check_output() and not check_gdb() and not check_radare():
		print "FOUND POSSIBLE FAIL\n\n\n"
		os.system("tail fuzz_gdb fuzz_radare")
		raw_input()
		
