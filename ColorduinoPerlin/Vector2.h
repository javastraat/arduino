float ease(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

class Vector2 {
	public:
		float x;
		float y;
		
		float dot(Vector2 v) {
			return x * v.x + y * v.y;
		}
		
		float influence(bool t[3]) {
			return 
				  (t[0] ? x : (1 - x))
				* (t[1] ? y : (1 - y));
		}
		
		Vector2 floor() {
			Vector2 o = { ::floor(x), ::floor(y) };
			return o;
		}
		
		Vector2 ease() {
			Vector2 o = { ::ease(x), ::ease(y) };
			return o;
		}
		
		Vector2 add(Vector2 v) {
			Vector2 o = { x + v.x, y + v.y };
			return o;
		}
		
		Vector2 add(bool v[3]) {
			Vector2 o = { x + (v[0] ? 1 : 0), y + (v[1] ? 1 : 0) };
			return o;
		}
		
		Vector2 sub(Vector2 v) {
			Vector2 o = { x - v.x, y - v.y };
			return o;
		}
		
		Vector2 sub(bool v[3]) {
			Vector2 o = { x - (v[0] ? 1 : 0), y - (v[1] ? 1 : 0) };
			return o;
		}
};
