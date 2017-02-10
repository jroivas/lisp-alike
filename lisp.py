#!/usr/bin/env python3
import math
import sys

def readfile(a):
    with open(a, 'r') as f:
        return f.read()

def atom(i):
    try:
        return int(i)
    except:
        try:
            return float(i)
        except:
            return i.strip()


def appendPartials(res, partials):
    if partials:
        at = atom(partials)
        partials = ''
        if type(at) == str and not at:
            return res, partials
        res.append(at)
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
            #res.append({'data': a})
            res.append(a)
        elif c == ')':
            res, partials = appendPartials(res, partials)
            return res, idx + 1
        else:
            partials += c
            idx += 1

    res, partials = appendPartials(res, partials)
    return res, idx

def evaluate(item, env, dp=0):
    if not item:
        return ''
    if type(item) == list and len(item) == 1:
        item = item[0]
    if type(item) == int or type(item) == float:
        return item
    elif item[0] == 'define':
        var = item[1]
        expr = item[2:]
        env[var] = evaluate(expr, env)
        return None
    elif type(item) == str:
        return env[item]
        #print ('%sItem : %s' % (dp*' ', tree.strip()))
        #return
        """
    elif type(item) == list:
        if len (item) == 1 and type(item[0]) == list:
            item = evaluate(item[0], env)
        #for i in item:
        #    evaluate(i, env)
        print (' ii' ,item)
        return item
        """
    else:
        res = evaluate(item[0], env)
        args = []
        for a in item[1:]:
            er = evaluate(a, env)
            if er is not None:
                args.append(er)
        p = res(*args)
        return p

    #elif item[0] == 'if':
    """
        for i in item:
            if type(i) == dict:
                d = i.get('data', None)
                v = i.get('value', None)
                if d:
                    evaluate(d, env, dp+1)
                elif v:
                    evaluate(v, env, dp+1)
                else:
                    raise ValueError('Invalid item: %s' % i)
            else:
                print ('%sINV : %s' % (dp*' ', i))
    """

if __name__ == '__main__':
    data = readfile(sys.argv[1])
    print (data)
    re, pos = parse(data)
    print (re)
    env = {
        'begin': lambda x: x,
        'pi': math.pi,
        '+': lambda x, y: x + y,
        '*': lambda x, y: x * y,
        'stack': []
    }
    print (evaluate(re, env))
