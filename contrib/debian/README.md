
Debian
====================
This directory contains files used to package transcendenced/transcendence-qt
for Debian-based Linux systems. If you compile transcendenced/transcendence-qt yourself, there are some useful files here.

## transcendence: URI support ##


transcendence-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install transcendence-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your transcendenceqt binary to `/usr/bin`
and the `../../share/pixmaps/transcendence128.png` to `/usr/share/pixmaps`

transcendence-qt.protocol (KDE)

