The command to run:
    make main (or)
    gcc -g main.c usersystem.c echo.c cd.c pwdd.c
    ./a.out

In the ls command, BLUE colour denotes directories, WHITE colour denotes executables and GREEN colour denotes the other colours.

In the discover command, I assumed that discover -df is valid and I also assumed that they can be in any order.

For the foreground and background commands I used execvp.

I assumed that flags can be given multiple times but even then they'll apply only once.

For the history command, I used a file with 20 '-'s and updated the last '-' whenever a new command comes, I updated it even when the command isn't valid.

Whenever the process runs in background,you won't get the system name and user name.

We will only get that the process has exited normally when we give a command(like in the linux terminal).

For spacing , I only considered the delimiters " " and "\t".

Please ignore the comments.

For the amount of time calculated by foreground process , I calculated the sum of times of all foreground processes.

Inappropriate ioctl for device error is popping up sometimes out of nowhere even though the command is getting executed. Please ignore it.

I assumed that ctrl+Z will stop all current running process (As per the actual terminal).

The TAB autocompletion is for the files or folders in that particular directory only. I assumed it because we were told that we should consider them according to the ls command during autofilling.

In the autocompletion command, nothing will printed and nothing will be done to the cursor when there are no files with the starting letters that we typed.

I used the raw code which was provided before for taking input character by character. Please do not plagiarise me in that part.

SPEC 1,2 and 3 in:
      tokenizer.c
SPEC 4 in:
      foreandback.c
SPEC 5 and 6 in:
      main.c
