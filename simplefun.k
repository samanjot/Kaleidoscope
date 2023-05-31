
extern putchard(d);

def putcharalpha(a) putchard(a+97);
def putnewline() putchard(13) : putchard(10);

def prova2(x y[])
  if x then 2 else 3 end
;

def g(x)
  var c[5], b = 0 in
    for a = 0, a < 5 in
      c[a] = a :
      b = b + c[a]
    end
  end
;

def simple(x y)
  var a = 0, b = 0 in
    while a < 5 in
      while b < 2 in
        x = x * 2 :
        b = b + 1 :
        putcharalpha(1)
      end :
      a = a + 1 :
      putcharalpha(0) :
      putnewline() :
      x = x + 0
    end
  end
;
