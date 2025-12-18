git init
git pull https://github.com/DiegoAG0708/holbertonschool-simple_shell.git
git add remote origin https://github.com/DiegoAG0708/holbertonschool-simple_shell.git
git remote add origin https://github.com/DiegoAG0708/holbertonschool-simple_shell.git
git branch
git branch -m main
git branch
ls
emacs shell.c
emacs shell.h
cd include
ls
emacs shell.h
cd
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 shell.c -I include -o shell
emacs shell.c
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 shell.c -I include -o shell
git add shell.c include/shell.h
git commit -m "Task 2: Simple Shell 0.1 with prompt, execve, and EOF handling"
git push origin main
git config --global credential.helper 'cache --timeout=7200'
mv include/shell.h .
gcc -o hsh -Wall -Werror -Wextra -pedantic -std=gnu89 *.c
emacs shell.c
emacs shell.h
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
emacs shell.h
mv include/shell.h .
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
rm main.c
mv main.c archive_main.c
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
git add shell.c shell.h
git commit -m "Fix: remove duplicate main.c to resolve multiple definition error"
git push origin main
git config --global credential.helper 'cache --timeout=7200'
emacs shell.c
emacs shell.h
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 shell.c -o hsh
./hsh
echo "/bin/ls" | ./hsh
git add shell.c shell.h README.md AUTHORS man_1_simple_shell
git commit -m "Task 2: Simple Shell 0.1 with prompt, execve, and EOF handling"
git push origin main
emacs shell.c
emacs shell.h
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 shell.c -o hsh
git add shell.c shell.h archive README.md AUTHORS man_1_simple_shell
git commit -m "Task 2: Simple Shell 0.1 minimal implementation; move extra sources to archive"
git push origin main
mkdir -p archive
mv *.c archive/
mv *.h archive/
mv shell.c archive/
mv shell.h archive/
cp archive/shell.c .
cp archive/shell.h .
pwd
ls -la
cd holbertonschool-simple_shell 2>/dev/null || git clone https://github.com/DiegoA9687/holbertonschool-simple_shell.git && cd holbertonschool-simple_shell
git branch
ls -1
mkdir -p archive
mv *.c archive/ 2>/dev/null
mv *.h archive/ 2>/dev/null
git checkout -- shell.c shell.h 2>/dev/null || true
mv main.c exec.c path.c parse.c builtins.c utils.c loop.c archive/ 2>/dev/null
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
ls -1 *.c
mv extra_name.c archive/
git add shell.c shell.h README.md AUTHORS man_1_simple_shell
git commit -m "Task 2: Minimal Simple Shell 0.1; only shell.c/shell.h in root"
git push origin main
mkdir -p archive
git mv -f builtins.c archive/ 2>/dev/null || true
git mv -f exec.c archive/ 2>/dev/null || true
git mv -f loop.c archive/ 2>/dev/null || true
git mv -f main.c archive/ 2>/dev/null || true
git mv -f parse.c archive/ 2>/dev/null || true
git mv -f path.c archive/ 2>/dev/null || true
git mv -f utils.c archive/ 2>/dev/null || true
# Move headers out of include/ into archive or root consistency
git mv -f include/shell.h shell.h 2>/dev/null || true
