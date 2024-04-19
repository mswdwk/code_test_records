# find java source file , not contains test file, statistics total lines.
find . -name "*.java" -a  ! -path "*test*" -exec cat {} \; |wc -l
# '-a' means 'and'
# '!' means 'not'
# '-exec' means 'execute command'
# '{} ' means 'the find out filename'
# '\;' means 'exec xxx command end'
