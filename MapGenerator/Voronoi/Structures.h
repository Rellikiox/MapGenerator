#pragma once

#include "../Math/Vec2.h"

struct arc;
struct seg;

struct event {
	double x;
	Vec2 p;
	arc *a;
	bool valid;

	event(double xx, Vec2 pp, arc *aa)
		: x(xx), p(pp), a(aa), valid(true) {}
};

struct arc {
	Vec2 p;
	arc *prev, *next;
	event *e;

	seg *s0, *s1;

	arc(Vec2 pp, arc *a=0, arc *b=0)
		: p(pp), prev(a), next(b), e(0), s0(0), s1(0) {}
};

struct seg {
	Vec2 start, end;
	bool done;

	seg(Vec2 p)
		: start(p), end(0,0), done(false) {}

	// Set the end point and mark as "done."
	void finish(Vec2 p) { if (done) return; end = p; done = true; }
};

struct gt {
	bool operator()(Vec2 a, Vec2 b) {return a.x==b.x ? a.y>b.y : a.x>b.x;}
	bool operator()(event *a, event *b) {return a->x>b->x;}
};