import cv2
import sys,os
import math

for f in sys.argv[1:]:
	img = cv2.imread(f, cv2.IMREAD_COLOR)
	work = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
	edges = cv2.Canny(work, 50, 200)
	lines = cv2.HoughLinesP(edges, 1, math.pi/180, 68, minLineLength=15, maxLineGap=250)

	sum_dx = 0
	sum_dy = 0
	sum_num = 0
	for line in lines:
		x1,y1,x2,y2 = line[0]
		cv2.line(img, (x1,y1), (x2,y2), (255,0,255), 1)

		assert(x1<=x2)

		dx = float(x2)-float(x1)
		dy = float(y2)-float(y1)
		if dx<dy: continue
		d = (dx**2 + dy**2) ** .5
		if d < 50: continue
		dx /= d
		dy /= d

		if dx < .9: continue
		sum_dx += dx
		sum_dy += dy
		sum_num += 1

	sum_dx /= sum_num
	sum_dy /= sum_num

	# using asin on dy instead of acos on dx gives signed angle
	angle = -math.asin(sum_dy)/math.pi*180

	#ff = os.path.splitext(f)[0] + "-deskew.png"
	ff = "deskew-%s" % f

	print(f, ff, angle)
	os.system("gm convert %s -rotate %f %s" % (f, angle, ff))

	#cv2.imwrite("dbglines-%s" % f, img)
