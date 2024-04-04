
# Unix ls Emulation program
  
  This program emulates the ls command in Unix. It supports the following options:
  1. -i: Display the inode number of each file.
  2. -l: Display detailed information about each file.
  
  The program requires that any options (-l, -i) come before any directory names.
  The file listing order does not have to be identical to ls.
  Dates are printed in the format "mmm dd yyyy hh:mm".
  When performing a long listing (-l), the "total" line is not printed.
  Files are printed one per line.
  
  To test the program, run it and specify the options the same way you would for the ls command.
  Test every option and all permutations of the options.
  
  Example test cases:
  - UnixLs -i ~/
  - UnixLs -l
  - UnixLs -i -l .. ~
  - UnixLs -li . .. ~
  
  Note: The output of the program should be similar to the original ls command, but the order of entries may be different.
  
  @version 1.0
  @date [current_date]
 