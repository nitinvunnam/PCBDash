with open("output.txt", "w") as f:
  print("{", end='', file=f)
  for i in range(60):
    for j in range(53):
      print (str(0xF81F)+',', end='', file=f)
  
  # for i in range(120):
  #   for j in range(20):
  #     print (str(0x0415)+',', end='', file=f)
  
  f.seek(f.tell() - 1)
  print("}", end='', file=f)