
def prova2(x y[])
  if x then 2 else 3 end
;

def prova(x y[])
  prova2(x, y) :
  var a[3], b[3] = {1,2,3} in
    y[x] = x :
    for y=x, y<10, 1 in
      while y>3 in
        y+1
      end
    end
  end
;
