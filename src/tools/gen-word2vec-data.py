import sys
import random

def gen_one_record(num_feas):
    feas = []
    target = random.random()
    for i in xrange(random.randint(6, num_feas)):
        id = random.randint(0, 300)
        feas.append("%d" % id)
    return " ".join(feas)
    

def gen_data(num):
    for i in xrange(num):
        line = gen_one_record(15)
        print line

if __name__ == '__main__':
    gen_data(10000)
