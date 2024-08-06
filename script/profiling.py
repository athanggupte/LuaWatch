#!/bin/python3.10
import timeit
import sys
import getopt

def main():

	try:
		opts, args = getopt.getopt(sys.argv[1:], "dn:u:r:vh",
									["debuggeroff", "number=", "repeat=",
									"verbose", "unit=", "help"])
	except getopt.error as err:
		print(err)
		print("use -h/--help for command line help")
		return 2

	stmt="subprocess.run(['lua', '{script_file}'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)"
	setup="import subprocess"
	repeat = 10
	number = 10000
	units = {"nsec": 1e-9, "usec": 1e-6, "msec": 1e-3, "sec": 1.0}
	time_unit = 'usec'
	precision = 6
	verbose = 0
	debugger = True

	for o, a in opts:
		if o in ("-d", "--debuggeroff"):
			debugger = False
		if o in ("-n", "--number"):
			number = int(a)
		if o in ("-u", "--unit"):
			if a in units:
				time_unit = a
			else:
				print("Unrecognized unit. Please select nsec, usec, msec, or sec.",
					file=sys.stderr)
				return 2
		if o in ("-r", "--repeat"):
			repeat = int(a)
			if repeat <= 0:
				repeat = 1
		if o in ("-v", "--verbose"):
			if verbose:
				precision += 1
			verbose += 1
		if o in ("-h", "--help"):
			print(__doc__, end=' ')
			return 0
		
	stmt = stmt.format(script_file="sample.lua" if debugger else "notasample.lua")

	raw_timings = timeit.repeat(
		stmt=stmt,
		setup=setup,
		repeat=repeat,
		number=number,
	)

	timings = [dt / number for dt in raw_timings]

	def format_time(dt):
		unit = time_unit

		if unit is not None:
			scale = units[unit]
		else:
			scales = [(scale, unit) for unit, scale in units.items()]
			scales.sort(reverse=True)
			for scale, unit in scales:
				if dt >= scale:
					break
		return "%.*g %s" % (precision, dt / scale, unit)

	best = min(timings)
	worst = max(timings)
	average = sum(timings) / repeat
	if worst >= best * 4:
		import warnings
		warnings.warn_explicit("The test results are likely unreliable. "
								"The worst time (%s) was more than four times "
								"slower than the best time (%s)."
								% (format_time(worst), format_time(best)),
								UserWarning, '', 0)


	if debugger:
		print("Test With Debugger")
	else:
		print("Test With/out Debugger")
	msg = f'''Run {number} loops; repeated {repeat} times:
		Best    : {format_time(best)}
		Average : {format_time(average)}
		Worst   : {format_time(worst)}
	'''

	print(msg)

if __name__ == '__main__':
	sys.exit(main())
