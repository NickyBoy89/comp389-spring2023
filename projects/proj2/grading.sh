#!/bin/sh
# Grading Guideline for Project 2
# ===============================

# Total points: 100

# +---------------------+
# | General Guidelines: |
# +---------------------+

# 1) The points below are maximum points.  Partial credits may be given.

# 2) Do the "plus points" first, then do the "minus points".

# 3) Each group's README will be read first to see if something different
#    needs to be done to get the code to work and what the known problems are.

# 4) The scripts below are for bash.

# 5) The grading is meant to be harsh!  So, if running the "diff" command
#    suppose to produce no output, but the group's code produces some
#    output, points will be taken off accordingly.  Similarly, if running
#    the "diff" command suppose to produce some output, but the group's
#    code produces no output, points will be taken off accordingly.

# 6) This grading guidelines is not perfect.  The grader will try his/her
#    best to stick to the grading guidelines.  Sometimes, after the grader
#    started grading some of the submissions, s/he realize that something
#    is missed (most often in "minus points").  The bottomline is that the
#    instructor reserves the right to add or change things in the grading
#    guidelines as he sees fit.

# +--------------+
# | Plus points: |
# +--------------+
 
# Documentation    : +3 points (comments in code plus README)

p0="yesnomaybe"
p1="comp389-f19"
p2="visual"
p3="blockcipher"
p4="emptystring"
p5="onetimepad"
p6="cryptography"
p7="publickey"

len0=320
len1=241
len2=202
len3=803
len4=724

# (A) stream: +10 points (2 points for each correct answer)
	echo "OPTION STREAM TEST"
	srcdir=testcases
	rm bar-s?.stream
	./proj2 stream -p="$p0" -l=$len0 > bar-s0.stream
	./proj2 stream -p="$p1" -l=$len1 > bar-s1.stream
	./proj2 stream -p="$p2" -l=$len2 > bar-s2.stream
	./proj2 stream -p="$p3" -l=$len3 > bar-s3.stream
	./proj2 stream -p="$p4" -l=$len4 > bar-s4.stream

	for f in 0 1 2 3 4
	do
		echo "==>$srcdir/s$f.stream"
		diff $srcdir/bar-s$f.stream bar-s$f.stream
	done


exit 1
	rm bar-s?.stream

# (B) encrypt: +24 points (3 points for each correct answer)
	echo "OPTION ENCRYPT TEST"
	srcdir=testcases
	/bin/rm bar*.pbm

	cat $srcdir/small.pbm | ./proj2 encrypt -p="$p0" -out=bar-small
	cat $srcdir/money.pbm | ./proj2 encrypt -p="$p1" -out=bar-money
	cat $srcdir/bowtie.pbm | ./proj2 encrypt -p="$p2" -out=bar-bowtie
	cat $srcdir/cross.pbm | ./proj2 encrypt -p="$p3" -out=bar-cross
	./proj2 encrypt -p="$p4" -out=bar-hello $srcdir/hello.pbm
	./proj2 encrypt -p="$p5" -out=bar-nouturn $srcdir/nouturn.pbm
	./proj2 encrypt -p="$p6" -out=bar-stop $srcdir/stop.pbm
	./proj2 encrypt -p="$p7" -out=bar-oxy $srcdir/oxy.pbm

	for f in small money bowtie cross hello nouturn stop oxy
	do
		echo "==> $srcdir/$f.pbm"
		diff $srcdir/bar-$f.1.pbm bar-$f.1.pbm
		diff $srcdir/bar-$f.2.pbm bar-$f.2.pbm
	done

# (C) merge: +24 points (3 points for each correct answer)
	echo "OPTION MERGE TEST"
	srcdir=testcases
	/bin/rm bar*.pbm

	for f in small money bowtie cross hello nouturn stop oxy
	do
		./proj2 merge $srcdir/bar-$f.1.pbm $srcdir/bar-$f.2.pbm > bar-merged.pbm
		echo "==> $srcdir/$f"
		diff $srcdir/bar-$f-merged.pbm bar-merged.pbm
		/bin/rm bar-merged.pbm
	done

# (D) decrypt: +24 points (3 points for each correct answer)
 	echo "OPTION DECRYPT TEST"
	srcdir=testcases
	/bin/rm bar*.pbm

	for f in small money bowtie cross
	do
		cat $srcdir/bar-$f-merged.pbm | ./proj2 decrypt > bar.pbm
		echo "==> $srcdir/$f.pbm"
		diff $srcdir/bar-$f.pbm bar.pbm
		/bin/rm bar.pbm
	done

	for f in hello nouturn stop oxy
	do
		./proj2 decrypt $srcdir/bar-$f-merged.pbm > bar.pbm
		echo "==> $srcdir/$f.pbm"
		diff $srcdir/bar-$f.pbm bar.pbm
		/bin/rm bar.pbm
	done

# (E) do the whole thing with student's code: +15 points
	echo "WHOLE PROCESS TEST"

	# (1) small.pbm: +5 points

		srcdir=testcases
		/bin/rm bar*.pbm

		./proj2 encrypt -p="$p0" -out=bar $srcdir/small.pbm
		./proj2 merge bar.1.pbm bar.2.pbm > bar-merged.pbm
		./proj2 decrypt bar-merged.pbm > bar.pbm

		echo "==> $srcdir/small.pbm "
		diff $srcdir/small.pbm bar.pbm

	# (2) hello.pbm: +5 points

		srcdir=testcases
		/bin/rm bar*.pbm

		cat $srcdir/hello.pbm | ./proj2 encrypt -p="$p1" -out=bar
		./proj2 merge bar.1.pbm bar.2.pbm | ./proj2 decrypt > bar.pbm

		echo "==> $srcdir/hello.pbm "
		diff $srcdir/hello.pbm bar.pbm

	# (3) oxy.pbm: +5 points

		srcdir=testcases
		/bin/rm bar*.pbm

		cat $srcdir/oxy.pbm | ./proj2 encrypt -p="$p2" -out=bar
		./proj2 merge bar.1.pbm bar.2.pbm | ./proj2 decrypt > bar.pbm

		echo "==> $srcdir/oxy.pbm "
		diff $srcdir/oxy.pbm bar.pbm

# +---------------+
# | Minus points: |
# +---------------+

# Missing README file  : -1 points (even if lots of comments in code)
#                        (Document+this cannot go negative)

# Improper citation    : -5 points
#                        for code obtained from a public source, the right place
#                        to cite the source is right next to the code

# Cannot compile       : -5 to -10, depending on effort to make it work
#                        if cannot generate executable, at least 5 points
#                        will be taken off, up to 10 points off if it
#                        takes a lot to make it work

# Compiler warnings    : -1 to -3
#                        if using gcc, make sure -Wall flag is used
#                        1 point will be taken off for 1-4 warnings,
#                        2 points off for 5-8 warnings, 3 points for more

# "make clean"         : -1 point if it does not work correctly

# Segmentation faults  : -10 points
#                        if a seg fault (or bus error) is seen any time during the
#                        testing of the code, if it's fairly repeatable, 10 points
#                        will be deducted, if it's intermittent, less will be deducted,
#                        depending on how often the seg faults are seen

# Separate compilation : -10 points
#                        if the executable is compiled with a single line, deduct
#                        all 10 points
#                        if the executable is a single module, but generates main.o
#                        and then link to create the executable in line one, deduct
#                        5 points
#                        if most of the code are in .h files, deduct all 10 points

# Too slow             : -10 points
#                        -2 points for each run that takes more than 5 minutes to complete

# Bad commandline      : -1 point each for each not handling properly
                       # need some sort of error output to indicate that the command
                       #     cannot be completed, it does not have to exactly the
                       #     error indicated below, just something reasonable to inform
                       #     the user about the error condition
                       # please note that commandline parsing is separate from
                       #     other functionalities, so even though a student has
                       #     declared that certain part of the assignments is not
                       #     implemented, commandling parsing still needs to be done
                       #     for those commands
					   echo "MALFORMED CMD TEST"
                       ./proj2
                       #    (malformed command)
                       ./proj2 stream -out=bar 
                       #    (malformed command)
                       ./proj2 stream -p="passphrase"
                       #    (malformed command)
                       ./proj2 encrypt
                       #    (malformed command)
                       ./proj2 merge -p="yesnomaybe"
                       #    (malformed command)
                       ./proj2 decrypt /usr/bin/xyzz
                       #    (input file /usr/bin/xyzz does not exist)

# Improper citation    : -20 points
                       #
                       # this is for the case that MOSS has found matched code and
                       # the studet did not cite downloaded/copied code properly
                       #
