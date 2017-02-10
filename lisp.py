#!/usr/bin/env python3
import sys

def readfile(a):
    with open(a, 'r') as f:
        return f.read()

def appendPartials(res, partials):
    if partials:
        res.append({'value': partials})
        partials = ''
    return res, partials

def parse(prg):
    res = []
    partials = ''
    idx = 0
    dest = len(prg)
    while idx < dest:
        c = prg[idx]
        #print ( '+cc %s %s %s' % (c,res, idx))
        if c == ' ':
            res, partials = appendPartials(res, partials)
            idx += 1
        elif c == '(':
            res, partials = appendPartials(res, partials)
            rest = prg[idx + 1:]
            a, b = parse(rest)
            idx += b + 1
            res.append({'data': a})
        elif c == ')':
            res, partials = appendPartials(res, partials)
            return res, idx + 1
        else:
            partials += c
            idx += 1

    res, partials = appendPartials(res, partials)
    return res, idx


def evaluate(tree, env):
    #for i in tree:
    idx = 0
    while idx < len(tree):
        i = tree[idx]
        d = i.get('data', None)
        v = i.get('value', None)
        if d:
            evaluate(d, env)
        elif v:
            evaluate(v, env)
            #if v.strip() == 'define':
        else:
            print(i)
        idx += 1
        #print ('A', i)

def genAST(tree):
    if type(tree) == str:
        print ('Item : %s' % tree.strip())
        return
    for i in tree:
        if type(i) == dict:
            d = i.get('data', None)
            v = i.get('value', None)
            if d:
                genAST(d)
            elif v:
                genAST(v)
            else:
                raise ValueError('Invalid item: %s' % i)
        else:
            print ('Item : %s' % i)

if __name__ == '__main__':
    data = readfile(sys.argv[1])
    print (data)
    re, pos = parse(data)
    print (re)
    env = {}
    genAST(re)
    #evaluate(re, env)
    #print ('---')
    #print (re)
