#include "cgs_rbt.h"
#include "cgs_rbt_private.h"
#include "cgs_test.h"

#include "cgs_variant.h"

enum node_flag { ADD, IGNORE };
enum node_child { LEFT, RIGHT, NUMCHILDREN };
struct node_data {
	int flag;
	int val;
	int color;
};

void set_children(struct cgs_rbt_node* node, struct node_data arr[NUMCHILDREN])
{
	struct cgs_variant v = { 0 };
	if (arr[LEFT].flag == ADD) {
		cgs_variant_set_int(&v, arr[LEFT].val);
		node->left = cgs_rbt_node_new(&v);
		node->left->color = arr[LEFT].color;
		node->left->parent = node;
	}
	if (arr[RIGHT].flag == ADD) {
		cgs_variant_set_int(&v, arr[RIGHT].val);
		node->right = cgs_rbt_node_new(&v);
		node->right->color = arr[RIGHT].color;
		node->right->parent = node;
	}
}

int rbt_rules_test(const struct cgs_rbt_node* node)
{
	// 3. Leaves are black (or NULL in this implementation)
	if (!node)
		return TEST_SUCCESS;

	// 1. Every node is either red or black
	if (node->color != CGS_RBT_RED && node->color != CGS_RBT_BLACK) {
		return TEST_FAILURE;
	}

	// 2. The root is black
	if (node->parent == NULL && node->color != CGS_RBT_BLACK)
		return TEST_FAILURE;

	// 4. If a node is red, both its children are black
	if (node->color == CGS_RBT_RED) {
		if (node->left->color != CGS_RBT_BLACK)
			return TEST_FAILURE;
		if (node->right->color != CGS_RBT_BLACK)
			return TEST_FAILURE;
	}

	// 5. All simple paths from a node have the same black-height
	int left_height = cgs_rbt_node_black_height(node->left);
	int right_height = cgs_rbt_node_black_height(node->right);
	if (left_height != right_height || left_height < 0)
		return TEST_FAILURE;

	return TEST_SUCCESS;
}

/* fake_tree_good_small
 *               _______________4B______________
 *              |                               |
 *       _______2R______                 _______6R______
 *      |               |               |               |
 *      0B__            3B              5B            __8B
 *          |                                        |
 *          1R                                       7R
 */
struct cgs_rbt_node* fake_tree_good_small()
{
	struct cgs_variant v = { 0 };
	cgs_variant_set_int(&v, 4);

	struct cgs_rbt_node* root = cgs_rbt_node_new(&v);
	root->color = CGS_RBT_BLACK;

	struct cgs_rbt_node* node = root;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 2, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 6, .color = CGS_RBT_RED }
	});

	node = node->left;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 0, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 3, .color = CGS_RBT_BLACK }
	});

	node = node->left;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = IGNORE },
		[RIGHT] = { .flag = ADD, .val = 1, .color = CGS_RBT_RED }
	});

	node = root->right;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 5, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 8, .color = CGS_RBT_BLACK }
	});

	node = node->right;
	set_children(node, (struct node_data[]){
		[LEFT]  = { .flag = ADD, .val = 7, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = IGNORE }
	});

	return root;
}

/* fake_tree_good_large
 *                      _____________26B_____________
 *                     |                             |
 *              ______17R______                _____41B______
 *             |               |              |              |
 *        ____14B___       ___21B___       __30R__          47B
 *       |          |     |         |     |       |
 *     _10R_      _16B   19B_      23B   28B    _38B_
 *    |     |    |           |                 |     |
 *   _7B   12B  15R         20R               35R   39R
 *  |
 *  3R
 *
 */
struct cgs_rbt_node* fake_tree_good_large()
{
	struct cgs_variant v = { 0 };
	cgs_variant_set_int(&v, 26);
	struct cgs_rbt_node* root = cgs_rbt_node_new(&v);
	root->color = CGS_RBT_BLACK;

	struct cgs_rbt_node* node = root; // 26
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 17, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 41, .color = CGS_RBT_BLACK }
	});

	node = root->left; // 17
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 14, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 21, .color = CGS_RBT_BLACK }
	});

	node = root->left->left; // 14
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 10, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 16, .color = CGS_RBT_BLACK }
	});

	node = root->left->left->left; // 10
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 7, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 12, .color = CGS_RBT_BLACK }
	});

	node = root->left->left->left->left; // 7
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 3, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = IGNORE }
	});

	node = root->left->left->right; // 16
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 15, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = IGNORE }
	});

	node = root->left->right; // 21
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 19, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 23, .color = CGS_RBT_BLACK }
	});

	node = root->left->right->left; // 19
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = IGNORE },
		[RIGHT] = { .flag = ADD, .val = 20, .color = CGS_RBT_RED },
	});

	node = root->right; // 41
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 30, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 47, .color = CGS_RBT_BLACK }
	});

	node = root->right->left; // 30
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 28, .color = CGS_RBT_BLACK },
		[RIGHT] = { .flag = ADD, .val = 38, .color = CGS_RBT_BLACK }
	});

	node = root->right->left->right; // 38
	set_children(node, (struct node_data[NUMCHILDREN]){
		[LEFT]  = { .flag = ADD, .val = 35, .color = CGS_RBT_RED },
		[RIGHT] = { .flag = ADD, .val = 39, .color = CGS_RBT_RED }
	});

	return root;
}

int rbt_node_small_hand_test(void* data)
{
	(void)data;

	struct cgs_rbt_node* root = fake_tree_good_small();

	assert(cgs_rbt_node_is_black(root));
	assert(cgs_rbt_node_is_red(root->left));
	assert(cgs_rbt_node_is_red(root->right));
	assert(cgs_rbt_node_is_black(root->left->left));
	assert(cgs_rbt_node_is_black(root->left->right));
	assert(cgs_rbt_node_is_black(root->right->left));
	assert(cgs_rbt_node_is_black(root->right->right));
	assert(cgs_rbt_node_is_red(root->left->left->right));
	assert(cgs_rbt_node_is_red(root->right->right->right));

	assert(cgs_rbt_node_black_height(root) == 3);

	assert(rbt_rules_test(root) == TEST_SUCCESS);

	cgs_rbt_node_free(root);

	return TEST_SUCCESS;
}

int rbt_node_large_hand_test(void* data)
{
	(void)data;
	struct cgs_rbt_node* root = fake_tree_good_large();

	assert(cgs_rbt_node_black_height(root) == 4);
	int left = cgs_rbt_node_black_height(root->left);
	int right = cgs_rbt_node_black_height(root->right);
	assert(left == right);

	assert(rbt_rules_test(root) == TEST_SUCCESS);

	cgs_rbt_node_free(root);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "rbt_node_small_hand", rbt_node_small_hand_test, NULL },
		{ "rbt_node_large_hand", rbt_node_large_hand_test, NULL },
	};

	return cgs_run_tests(tests);
}

