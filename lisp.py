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


def parseAtom(val, cdr, car):
    val = atom(val)
    if val is not None and val != '':
        if cdr is None:
            cdr = val
        elif car is None:
            car = val
        else:
            car = (car, val)
    val = ''

    return (val, cdr, car)

def parsePair(prg):
    res = []
    partials = ''
    idx = 0
    dest = len(prg)
    cdr = None
    car = None
    while idx < dest:
        c = prg[idx]
        #print ( '+cc %s %s %s' % (c,res, idx))
        if c == '"' or (partials and partials[0] == '"'):
            if partials:
                if partials[0] == '"':
                    partials += c
                if c == '"':
                    # FIXME
                    partials, cdr, car = parseAtom(partials, cdr, car)
                    partials = ''
            else:
                partials = '"'
            idx += 1
        elif c == ' ':
            partials, cdr, car = parseAtom(partials, cdr, car)
            #print ('GGP %s, %s' % (cdr, car))
            #partials = ''
            idx += 1
        elif c == '(':
            partials, cdr, car = parseAtom(partials, cdr, car)
            #print ('PPP %s, %s' % (cdr, car))
            rest = prg[idx + 1:]
            #a, b = parsePair(rest)
            #idx += b + 1
            a, b, c = parsePair(rest)
            idx += c + 1
            #print ('CCC' , a,b)
            if cdr is None:
                cdr = a
                car = b
            elif car is None:
                car = (a, b)
            else:
                cdr = (cdr, car)
                car = (a, b)
                #car = (car, (a, b))
            #print ('CCP %s, %s' % (cdr, car))
        elif c == ')':
            partials, cdr, car = parseAtom(partials, cdr, car)
            #print ('RRP %s, %s' % (cdr, car))
            return (cdr, car, idx + 1)
            #return res, idx + 1
        else:
            partials += c
            idx += 1

    partials, cdr, car = parseAtom(partials, cdr, car)
    return (cdr, car, idx)
    #return res, idx

class LambdaCall(object):
    def __init__(self, body, params, env):
        self.params = params
        self.body = body
        self.env = env

    def __call__(self, *args):
        #return stackEval(self.body, Env(env, self.params, args))
        return stackEval(self.body, Env(env, self.params, args))

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

def parseArgs(v, env):
    res = []
    e = stackEval(v, env)
    res.append(e)
    return res

def mapOper(oper, var, env):
    opermap = {
        '+': lambda a, b: a+b,
        '-': lambda a, b: a-b,
        '*': lambda a, b: a*b,
        '/': lambda a, b: a/b
    }
    if type(var) == tuple:
        l = stackEval(var[0], env)
        r = stackEval(var[1], env)
    else:
        l = stackEval(var, env)
        r = 0
    if type(l) == tuple:
        l = opermap[oper](l[0], l[1])
    if type(r) == tuple:
        r = opermap[oper](r[0], r[1])
    res= opermap[oper](l, r)
    print (oper, res)
    return res
    """
    print (var)
    if type(var) == tuple:
        r = stackEval(var[0], env)
        l = stackEval(var[1], env)
        #print (r,l)
        return (r, l)
    else:
        print (var)
        return stackEval(var, env)

    #print (r, l)
    """

def stackEval(val, env):
    if type(val) == tuple:
        cdr, car = val
        if type(cdr) == tuple:
            print ('CC' , cdr)
            l = stackEval(cdr, env)
            r = stackEval(car, env)
            return r
            print ('LR' , l,r)
            if l is not None and r is not None:
                return (l, r)
                #return r
                raise ValueError('Unexpected: %s %s' % (l, r))
            elif r is not None:
                return r
        elif cdr == 'begin':
            return stackEval(car, env)
        elif cdr == 'define':
            if type(car) == tuple:
                var = car[0]
                if type(var) == tuple:
                    name = var[0][0]
                    args = var[0][1]
                    #print ('DDD', stackEval(var, env))
                    #body = stackEval(var[1], env)
                    body = var[1]
                    env[name] = LambdaCall(body, args, env)
                else:
                    val = stackEval(car[1], env)
                    env[var] = val
                return stackEval(car[1], env)
            else:
                raise ValueError('Unexpected define: %s' % val)
        elif cdr in ['+', '-', '*', '/']:
            #return stackEval(car[0], env) * stackEval(car[1], env)
            return mapOper(cdr, car, env)
        elif cdr == 'or':
            return stackEval(car[0], env) or stackEval(car[1], env)
        elif cdr == 'equal?' or cdr == '=':
            return stackEval(car[0], env) == stackEval(car[1], env)
        elif cdr == 'cos':
            return math.cos(stackEval(car, env))
        elif cdr == 'if':
            test = car[0][0]
            then = car[0][1]
            elsed = car[1]
            res = stackEval(test, env)
            if res:
                e = stackEval(then, env)
            else:
                e = stackEval(elsed, env)
            return e
        elif env.find(cdr):
            item = env.find(cdr)
            #print ('CALL %s' %  cdr)
            #args = []
            args = parseArgs(car, env)
            #print ('CALL %s (%s)' %  (cdr, args))
            #stackEval(car, env)
            #if type(args) != tuple:
            #    args = [args]
            return item(*args)
        else:
            #print ('APE', cdr)
            #print ('APE', car)
            l = stackEval(cdr, env)
            r = stackEval(car, env)
            #return val
            #raise ValueError('Unexpected tuple: %s' % str(val))
            return (l, r)
        #print (stackEval(car, env))
    elif type(val) == int or type(val) == float:
        return val
    elif type(val) == str:
        v = env.find(val)
        if v is None:
            raise ValueError('Undefined variable: %s' % val)
        return v
    else:
        raise ValueError('Unexpected entry: %s' % val)

if __name__ == '__main__':
    data = readfile(sys.argv[1])
    #print (data)

    cdr, car, pos = parsePair(data)
    prg = (cdr, car)
    #print (cdr,car)

    env = Env()
    env.update(vars(math))

    #print (' Rp ', prg)
    print (stackEval(prg, env))
