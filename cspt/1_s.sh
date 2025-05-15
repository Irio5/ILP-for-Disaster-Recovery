x="$00"
for g in *.conf
  do
  temp="${g%%.*}"
  temp1="$(cut -d'_' -f3 <<<"$temp")"
  if [ "$x" = "$temp1" ]
  then
    ls carrier_*_$x.*
  fi
  done
  
  

