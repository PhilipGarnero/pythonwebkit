#
# Regular cron jobs for the pythonwebkit package
#
0 4	* * *	root	[ -x /usr/bin/pythonwebkit_maintenance ] && /usr/bin/pythonwebkit_maintenance
