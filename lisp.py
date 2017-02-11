#!/usr/bin/env python3
import math
import operator
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
        if c == '"' or (partials and partials[0] == '"'):
            if partials:
                if partials[0] == '"':
                    partials += c
                if c == '"':
                    res, partials = appendPartials(res, partials)
                    partials = ''
            else:
                partials = '"'
            idx += 1
        elif c == ' ':
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

class LambdaCall(object):
    def __init__(self, body, params, env):
        self.params = params
        self.body = body
        self.env = env

    def __call__(self, *args):
        return evaluate(self.body, Env(env, self.params, args))

class Env(dict):
    def __init__(self, parent=None, args=[], vals=()):
        #print (' EEE %s pp %s' % (args, vals))
        self.update(zip(args, vals))
        self.parent = parent

    def find(self, var):
        if var in self:
            return self[var]
        elif self.parent:
            return self.parent.find(var)
        
        return None

def evaluate(item, env, dp=0):
    #print ('TTT %s == %s' % (type(item), item))
    if item is None or item == '':
        return ''
    if type(item) == list and len(item) == 1:
        item = item[0]
    if type(item) == int or type(item) == float:
        return item
        """
    elif item[0] == 'defun':
        parms = item[1]
        body = item[2:]
        return {'params': parms, 'body':body}
        #return {'params': params, 'body':body}
        #env[var] = evaluate(expr, env)
        return None
        """
    elif item[0] == 'define':
        var = item[1]
        if type(var) == list:
            # Function def
            name = var[0]
            params = var[1:]
            body = item[2:]
            env[name] = LambdaCall(body, params, env)
        else:
            expr = item[2:]
            env[var] = evaluate(expr, env)
        #print ('DDD %s %s' % (var, expr))
        return None
    elif item[0] == 'if':
        test = item[1]
        then = item[2]
        elsed = item[3]
        case = evaluate(test, env)
        ret = None
        #print (' CC %s == %s, %s' % (case, test, env.find('N')))
        if case:
            #print (' TH %s' % then)
            ret = then
        else:
            #print (' EL %s' % elsed)
            ret = elsed
        return evaluate(ret, env)
    elif type(item) == str:
        if item[0] == '"':
            return None
        return env.find(item)
    else:
        res = evaluate(item[0], env)
        args = []
        for a in item[1:]:
            er = evaluate(a, env)
            if er is not None:
                args.append(er)
        #print ('AAA %s' % args)
        #print ('rrr %s' % res)
        if res is None:
            if args:
                return args[0]
            return None
        p = res(*args)
        return p

if __name__ == '__main__':
    data = readfile(sys.argv[1])
    #print (data)
    re, pos = parse(data)
    #print (re)
    env = Env()
    env.update(vars(math))
    env.update({
        'begin': lambda x: x,
        'or': lambda x, y: x or y,
        'equal?': lambda x, y: x == y,
        '=': lambda x, y: x == y,
        '+': operator.add,
        '-': operator.sub,
        '*': operator.mul,
        '/': operator.truediv,
        'none': None
    })
    print (evaluate(re, env))
