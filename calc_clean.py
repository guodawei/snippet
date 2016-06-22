org = '2*.3/-0.9/5*6'
tokens = org.replace('+',' + ').replace('-',' - ').replace('*',' * ')\
            .replace('/',' / ').replace('(',' ( ').replace(')',' ) ').split()
tokens.append(";")
idx = 0
def Curr():
    global idx, tokens
    return tokens[idx]
def Next(char=''):
    global idx, tokens
    curr = Curr()
    if char!='' and char!=curr:
        print "Expect ", char, ", while there is nothing here";exit()
    idx = idx + 1
    return curr
def Expr():
    left = Term()
    while True:
        if   '+' == Curr(): Next('+'); left += Term()
        elif '-' == Curr(): Next('-'); left -= Term()
        else: return left
def Term():
    left = Prim()
    while True:
        if   '*' == Curr(): Next('*'); left *= Prim()
        elif '/' == Curr(): Next('/'); left /= Prim()
        else: return left
def Prim():
    if is_num(Curr()): return float(Next())
    if is_min(Curr()): Next('-'); return -float(Next())
    if is_lpa(Curr()): Next('('); expr = Expr(); Next(')'); return expr
def is_num(x): return x[0] in ".0123456789"
def is_min(x): return x == '-'
def is_lpa(x): return x == '('
print Expr()
