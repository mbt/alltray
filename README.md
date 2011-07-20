AllTray
=======

This is AllTray.  AllTray is an application which docks other
application software into the system tray (“notification area”) of a
desktop environment such as GNOME, KDE, or XFCE. It aims to be
independent of both desktop environment and window manager. Old
releases are available, and currently the project is focusing on
updating the source code to become more fully compliant with existing
desktop standards, as well as being refactored.

Requirements
------------

See the REQUIREMENTS file in the tree to see what build and run-time
requirements are.  However, as long as you have all of the
requirements met, AllTray will build with the standard `configure &&
make && make install` process.

Feedback
--------

AllTray is currently in the process of moving to GitHub.  Much of its
infrastructure has been hosted at Launchpad for the past few years,
which was a significant improvement from where it was at Sourceforge.
Of course, Launchpad is built around the Bazaar DVCS, not git, so it
is not likely that it will continue to be the home for AllTray (aside
perhaps from translations, but that would require that there be some
sort of really good bidirectional interface for git↔bzr interaction;
git-bzr is not that).

History and Future
------------------

AllTray has had a very long history.  It was originally written by
Jochen Baier, who (as best as I can tell) has left the world of UNIX
and UNIX-like software and gone to the world of Windows.  In late 2008
I took over AllTray and began to rewrite it to update it to better
align with modern standards found in the X11 environment.

The new development branch of AllTray, the 0.7.Xdev series, has taken
a very long time to work on.  However, it is nearing completion:
several key features have been reimplmented, and old features that no
longer make sense have been removed.  No longer does AllTray rely on
window-manager specific tricks to function, which should improve the
experience for everyone.  That is not to say that new AllTray is
bug-free: it almost certainly is not.

Version 0.7.5dev was released on July 14, 2011.  Work on 0.7.6dev is
nearing completion already as of July 20, 2011.  0.7.6dev will be
released within the next couple of weeks, and it is likely to be the
last development release before the next stable—the first stable
release of AllTray since I took it over in 2008.  Exciting!

Contributions
-------------

Of course, I welcome any contributions.  Please send pull requests if
you have contributions!  :-)