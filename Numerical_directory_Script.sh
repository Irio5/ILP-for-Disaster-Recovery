for i in {01..09}
do 
  cd Instances_$i
  cd DCI_Analysis
  cd DCP1
  mkdir ILP
  mkdir KSP
  cd .. 
  cd DCP2
  mkdir ILP
  mkdir KSP
  cd ..
  cd ..
  cd ..
done

for i in {10..449}
do 
  cd Instances_$i
  cd DCI_Analysis
  cd DCP1
  mkdir ILP
  mkdir KSP
  cd .. 
  cd DCP2
  mkdir ILP
  mkdir KSP
  cd ..
  cd ..
  cd ..
done
