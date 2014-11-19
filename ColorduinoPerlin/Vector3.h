float ease(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

class Vector3 {
	public:
		float x;
		float y;
		float z;
		
		float dot(Vector3 v) {
			return x * v.x + y * v.y + z * v.z;
		}
		
		float influence(bool t[3]) {
			return 
				  (t[0] ? x : (1 - x))
				* (t[1] ? y : (1 - y))
				* (t[2] ? z : (1 - z));
		}
		
		Vector3 floor() {
			Vector3 o = { ::floor(x), ::floor(y), ::floor(z) };
			return o;
		}
		
		Vector3 ease() {
			Vector3 o = { ::ease(x), ::ease(y), ::ease(z) };
			return o;
		}
		
		Vector3 add(Vector3 v) {
			Vector3 o = { x + v.x, y + v.y, z + v.z };
			return o;
		}
		
		Vector3 add(bool v[3]) {
			Vector3 o = { x + (v[0] ? 1 : 0), y + (v[1] ? 1 : 0), z + (v[2] ? 1 : 0) };
			return o;
		}
		
		Vector3 sub(Vector3 v) {
			Vector3 o = { x - v.x, y - v.y, z - v.z };
			return o;
		}
		
		Vector3 sub(bool v[3]) {
			Vector3 o = { x - (v[0] ? 1 : 0), y - (v[1] ? 1 : 0), z - (v[2] ? 1 : 0) };
			return o;
		}
};
