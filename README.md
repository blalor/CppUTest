CppUTest
========

From the [project page](http://www.cpputest.org/):

> CppUTest is a C /C++ based unit xUnit test framework for unit testing and for test-driving your code. It is written in C++ but is used in C and C++ projects and frequently used in embedded systems.

This "fork" contains my tweaks to make it more usable for me.

`git svn` config
----------------

The svn trunk is cloned on the `svn_upstream` branch.  Only perform `git svn rebase` on that branch.  Never attempt to `git svn dcommit` on that or any other branch, unless you know what you're doing (and **I** certainly don't).  You should then be able to merge from `svn_upstream` to `master`.

Create `.git/gitsvn_authors` with the following contents:

    basvodde = Bas Vodde <basv@odd-e.com>
    jamesgrenning = James Grenning <info@renaissancesoftware.net>
    tpuronen = Timo Puronen <timo.puronen@reaktor.fi>

Add the following sections to `.git/config` (it may be necessary to change the path to `authorsfile`):

    [svn-remote "svn"]
        url = https://cpputest.svn.sourceforge.net/svnroot/cpputest/trunk
        fetch = :refs/remotes/git-svn
    [svn]
        authorsfile = .git/gitsvn_authors

