with open("output.txt", "w") as f:
  print("{", end='', file=f)
  for i in range(20):
    for j in range(20):
      print (str(0x03fd)+',', end='', file=f)
  
  for i in range(120):
    for j in range(20):
      print (str(0x0415)+',', end='', file=f)
  
  f.seek(f.tell() - 1)
  print("}", end='', file=f)