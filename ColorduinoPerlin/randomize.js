/* Permutations Table */

var permutations = [];
for(var i = 0; i < 256; i++) permutations.push(i);

for(var i = permutations.length - 1; i > 0; i--) {
	var j = Math.floor(Math.random() * i);
	
	var temp = permutations[i];
	permutations[i] = permutations[j];
	permutations[j] = temp;
}

console.log('unsigned char permutations[] = {' + permutations.join(', ') + '};');


/* Gradients Table */

var gradients = [];
for(var i = 0; i < 256; i++) gradients.push(Math.random() * 2 - 1);

console.log('float gradients[] = {' + gradients.join(', ') + '};');
