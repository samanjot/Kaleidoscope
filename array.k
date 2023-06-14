extern printd(d);

def squareSum(n)
    var a[n], sum = 0 in
        for i = 1, i <= n in
            a[i-1] = i * i
        end
        :
        for i = 0, i < n in
            printd(a[i])
        end
        :
        for i = 0, i < n in
            sum = sum + a[i]
        end
    end
;

def search(a[] n field)
    var i = 0, found = -1 in 
        while i <= n in
            if a[i] == field then
                found = i
                :
                i = n
            else
                found = -1
            end
            :
            i = i + 1
        end
        :
        found
    end
;

def fibonacci(n)
    if n < 0 then 0
    else if n <= 1 then 1
        else fibonacci(n-1)+fibonacci(n-2) end
    end
;

def simple(a[] n m)
0;

def sort(a[] n)
  var tmp = 0, idx = 0 in
    for i = 0, i < n - 1 in
      idx = i :
      for j = i + 1, j < n in
        idx = if a[idx] > a[j] then j else idx end
      end :
      tmp = a[idx] :
      a[idx] = a[i] :
      a[i] = tmp
    end
  end
;

def sum()
    var arr[5] = {5, 4, 3, 2, 1}, sum = 0 in 
        sort(arr, 5)
        :
        for i = 0, i < 5 in
            printd(arr[i])
        end
        :
        for i = 0, i < 5 in
            sum = sum + arr[i]
        end
    end
;


def sum2()
    var sum = 0 in
        for i = 0, i <= 20, 2 in 
            sum = sum + i
        end
    end
;