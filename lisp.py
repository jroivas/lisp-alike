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
                    res, partials = appendPartials(res, partials)
                    partials = ''
            else:
                partials = '"'
            idx += 1
        elif c == ' ':
            #res, partials = appendPartials(res, partials)
            partials, cdr, car = parseAtom(partials, cdr, car)
            #print ('GGP %s, %s' % (cdr, car))
            #partials = ''
            #print ('GGG %s,%s'% (res, partials))
            idx += 1
        elif c == '(':
            partials, cdr, car = parseAtom(partials, cdr, car)
            #print ('PPP %s, %s' % (cdr, car))
            #res, partials = appendPartials(res, partials)
            #print ('PPP %s,%s'% (res, partials))
            rest = prg[idx + 1:]
            #a, b = parsePair(rest)
            #idx += b + 1
            a, b, c = parsePair(rest)
            idx += c + 1
            if cdr is None:
                cdr = a
                car = b
            elif car is None:
                car = (a, b)
            else:
                car = (car, (a, b))
            #res.append({'data': a})
            #res.append(a)
        elif c == ')':
            #res, partials = appendPartials(res, partials)
            partials, cdr, car = parseAtom(partials, cdr, car)
            #print ('RRR %s,%s'% (res, partials))
            #print ('RRP %s, %s' % (cdr, car))
            return (cdr, car, idx + 1)
            #return res, idx + 1
        else:
            partials += c
            idx += 1

    #res, partials = appendPartials(res, partials)
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
    return opermap[oper](l, r)
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
            l = stackEval(cdr, env)
            r = stackEval(car, env)
            return r
            if l is not None and r is not None:
                #return (l, r)
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
    #print (re)
    env = Env()
    env.update(vars(math))
    """
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
    """
    #print (evaluate(re, env))
    #print (' Rp ', prg)
    print (stackEval(prg, env))
