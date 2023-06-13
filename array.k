def squareSum(n)
    var a[n], sum = 0 in
        for i = 1, i <= n in
            a[i-1] = i * i
        end
        :
        for i = 0, i < n in
            sum = sum + a[i]
        end
        :
        sum
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