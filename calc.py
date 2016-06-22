S = '1+((2+3))+((((4))))*.5*6.6/-7*8/9'
def calc(s):
    hw = "(+-*/);"
    global i; i = 0
    def C():
        global i; return t[i]
    def N():
        global i; c = t[i]; i += 1; return c
    def E():
        t = T()
        while True:
            if   C()=='+': N(); t+=T()
            elif C()=='-': N(); t-=T()
            else: return t
    def T():
        p = P()
        while True:
            if   C()=='*': N(); p*=P()
            elif C()=='/': N(); p/=P()
            else: return p
    def P():
        if (lambda x:x[0] not in hw)(C()): return float(N())
        if (lambda x:x == '-')(C()): N(); return -float(N())
        if (lambda x:x == '(')(C()): N();e=E();N(); return e
    t = reduce(lambda x,y:x+y, map(lambda x:[x,' '+x+' '][x in hw], s+';')).split()
    return E()
def core_calc(s): return eval(s)
print calc(S);print core_calc(S)
