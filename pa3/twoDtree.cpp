
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"
#include <iostream>
#include <math.h>
/* given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

/* given */
twoDtree::~twoDtree(){
	clear();
}

/* given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}

/* given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

twoDtree::twoDtree(PNG & imIn){ /* your code here */
	 stats s = stats(imIn);
	 pair<int, int> ul (0,0);
	 pair<int, int> lr (imIn.width() - 1 , imIn.height() - 1);
	 height = imIn.height();
	 width = imIn.width();
	 root = buildTree(s, ul, lr);
}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	/* your code here */
	Node* temp = new Node(ul, lr, s.getAvg(ul, lr));
	if(ul.first == lr.first && ul.second == lr.second)
		return temp;
	pair<int, int> newul = getSmall(s, ul, lr).first;
	pair<int, int> newlr = getSmall(s, ul, lr).second;
	temp->left = buildTree(s, ul, newlr);
	temp->right = buildTree(s, newul, lr);
	return temp;
}

PNG twoDtree::render(){
	/* your code here */
	pair<int, int> lowRight = root -> lowRight;
	PNG out = PNG(width, height);
	render_help(root, out);
	return out;
}

void twoDtree::render_help(Node* subtree, PNG & out) {
	if(subtree == NULL)
		return;
	if(subtree->left == NULL && subtree->right == NULL){
			draw(subtree, out);
	}
	render_help(subtree->left, out);
	render_help(subtree->right, out);


}

void twoDtree::draw (Node* subtree, PNG & out) {
	for (int x = subtree->upLeft.first; x <= subtree->lowRight.first; x++)
		for(int y = subtree->upLeft.second; y <= subtree->lowRight.second; y++){
			RGBAPixel* pixel = out.getPixel(x, y);
			*pixel = subtree->avg;
		}
}

void twoDtree::prune(double pct, int tol){
	/* your code here */
	prune_helper(pct, tol, root);
}

void twoDtree::prune_helper(double pct, int tol, Node* subtree){
 if(subtree == NULL)
	 return;
 vector<int> c = isPrune(tol, subtree);
 double apct = c[1]/(c[0] * 1.0);
 if(apct >= pct){
	 clear_helper(subtree->left);
	 subtree->left = NULL;
	 clear_helper(subtree->right);
	 subtree->right = NULL;
	 subtree = NULL;
	 return;
 }
 prune_helper(pct, tol, subtree->left);
 prune_helper(pct, tol, subtree->right);
}

 vector<int> twoDtree::isPrune(int tol, Node* subtree){
	vector<int> count; count.push_back(1); count.push_back(0);
	isPrune_helper(count, tol, subtree, subtree->left);
	isPrune_helper(count, tol, subtree, subtree->right);
	return count;
}

void twoDtree::isPrune_helper(vector<int> & count, int tol, Node* root, Node* subtree){
	if(subtree == NULL)
	 return;
	count[0]++;
	RGBAPixel rootPixel = root->avg;
	RGBAPixel subtreePixel = subtree->avg;
	int distance = pow((rootPixel.r - subtreePixel.r), 2.0) + pow((rootPixel.g - subtreePixel.g), 2.0) + pow((rootPixel.b - subtreePixel.b), 2.0);
	if(distance <= tol)
	 count[1]++;
	isPrune_helper(count, tol, root, subtree->left);
	isPrune_helper(count, tol, root, subtree->right);

}

void twoDtree::clear() {
	/* your code here */
	clear_helper(root);
}

void twoDtree::clear_helper(Node* subtree){
	if(subtree == NULL)
		return;
	clear_helper(subtree->left);
	clear_helper(subtree->right);
	delete(subtree);
	if(subtree->left != NULL)
		subtree->left = NULL;
	if(subtree->right != NULL)
		subtree->right = NULL;
	subtree = NULL;
}


void twoDtree::copy(const twoDtree & orig){
	/* your code here */
	this->root = new Node(orig.root->upLeft, orig.root->lowRight, orig.root->avg);
	this->height= orig.height;
	this->width= orig.width;
	copy_helper(orig.root, this->root);
}

void twoDtree::copy_helper(Node* orgTree, Node* copyTree){
	if(orgTree == NULL)
	 return;
	if(orgTree->left != NULL){
	copyTree -> left = new Node(orgTree->left->upLeft, orgTree->left->lowRight, orgTree->left->avg);
	copy_helper(orgTree->left, copyTree->left);
	}
	if(orgTree->right != NULL){
	copyTree -> right = new Node(orgTree->right->upLeft, orgTree->right->lowRight, orgTree->right->avg);
	copy_helper(orgTree->right, copyTree->right);
	}

}



pair< pair<int, int>, pair<int, int> > twoDtree::getSmall(stats & s, pair<int, int> ul, pair<int, int> lr) {
	 long Score = s.getScore(ul, lr);
	 pair< pair<int, int>, pair<int, int> > hp;
	 pair<int, int> defaultlr = lr;
	 pair<int, int> defaultul = ul;
	 if(lr.first - ul.first > 0 && (lr.first - ul.first) > (lr.second - ul.second)){
	 	defaultlr = make_pair(ul.first + ((lr.first - ul.first)/2) , lr.second);
		defaultul = make_pair(ul.first + ((lr.first - ul.first)/2) + 1, ul.second);
	}
	else if(lr.second - ul.second > 0){
		defaultlr = make_pair(lr.first, ul.second +  ((lr.second - ul.second)/2));
		defaultul = make_pair(ul.first, ul.second +  ((lr.second - ul.second)/2) + 1);
	}
	hp = make_pair(defaultul, defaultlr);


	 if(ul.second != lr.second){
	 	for(int i = ul.second; i < lr.second; i++){
		 	pair<int, int> newlr;
		 	pair<int, int> newul;
		 	newlr = make_pair(lr.first, i);
		 	newul = make_pair(ul.first, i + 1);
		 	long temp = s.getScore(ul, newlr) + s.getScore(newul, lr);
		 	if(temp < Score){
		  	hp = make_pair(newul, newlr);
				Score = temp;
			}
	 }
 }
	if(ul.first != lr.first){
	 for(int i = ul.first; i < lr.first; i++){
		 pair<int, int> newlr;
		 pair<int, int> newul;
		 newlr = make_pair(i, lr.second);
		 newul = make_pair(i + 1, ul.second);
		 long temp = s.getScore(ul, newlr) + s.getScore(newul, lr);
		 if(temp < Score){
		  	hp = make_pair(newul, newlr);
				Score = temp;
			}
	 }
 }
	return hp;
 }

void twoDtree::test(){
	int count = 0;
	int* pcount = &count;
	test_help(root, pcount);
	std::cout << "count: "<<count << '\n';
}

 void twoDtree::test_help(Node* subtree, int* count){
	 if(subtree == NULL)
	  return;
	if(subtree->left == NULL && subtree->right == NULL){
	 std::cout << "UL: "<<subtree->upLeft.first<<", "<< subtree->upLeft.second << '\n';
	 std::cout << "LR: "<<subtree->lowRight.first<<", "<< subtree->lowRight.second << '\n';
	 *count += 1;
 }
	 test_help(subtree->left, count);
	 test_help(subtree->right, count);
 }
