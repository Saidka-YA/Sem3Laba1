// Узел бинарного дерева поиска.
struct TreeNode {
    key: i32,
    left: Option<Box<TreeNode>>,
    right: Option<Box<TreeNode>>,
}

#[derive(Default)]
pub struct BinarySearchTree {
    root: Option<Box<TreeNode>>,
}

impl BinarySearchTree {
    pub fn insert(&mut self, key: i32) {
        insert_node(&mut self.root, key);
    }

    pub fn contains(&self, key: i32) -> bool {
        let mut node = self.root.as_deref();
        while let Some(current) = node {
            if key == current.key {
                return true;
            }
            node = if key < current.key {
                current.left.as_deref()
            } else {
                current.right.as_deref()
            };
        }
        false
    }

    pub fn delete(&mut self, key: i32) -> bool {
        let (root, removed) = remove_node(self.root.take(), key);
        self.root = root;
        removed
    }

    pub fn print_inorder(&self) {
        let mut keys = Vec::new();
        inorder(self.root.as_deref(), &mut keys);
        println!(
            "{}",
            keys.iter()
                .map(i32::to_string)
                .collect::<Vec<_>>()
                .join(" ")
        );
    }

    pub fn print_preorder(&self) {
        let mut keys = Vec::new();
        preorder(self.root.as_deref(), &mut keys);
        println!(
            "{}",
            keys.iter()
                .map(i32::to_string)
                .collect::<Vec<_>>()
                .join(" ")
        );
    }

    pub fn print_postorder(&self) {
        let mut keys = Vec::new();
        postorder(self.root.as_deref(), &mut keys);
        println!(
            "{}",
            keys.iter()
                .map(i32::to_string)
                .collect::<Vec<_>>()
                .join(" ")
        );
    }
}

fn insert_node(node: &mut Option<Box<TreeNode>>, key: i32) {
    match node {
        None => {
            *node = Some(Box::new(TreeNode {
                key,
                left: None,
                right: None,
            }))
        }
        Some(current) if key < current.key => insert_node(&mut current.left, key),
        Some(current) if key > current.key => insert_node(&mut current.right, key),
        Some(_) => {}
    }
}

fn remove_node(node: Option<Box<TreeNode>>, key: i32) -> (Option<Box<TreeNode>>, bool) {
    let mut node = match node {
        Some(node) => node,
        None => return (None, false),
    };

    if key < node.key {
        let (left, removed) = remove_node(node.left.take(), key);
        node.left = left;
        return (Some(node), removed);
    }
    if key > node.key {
        let (right, removed) = remove_node(node.right.take(), key);
        node.right = right;
        return (Some(node), removed);
    }

    match (node.left.take(), node.right.take()) {
        (None, right) => (right, true),
        (left, None) => (left, true),
        (left, Some(right)) => {
            let (new_right, mut successor) = take_min(right);
            successor.left = left;
            successor.right = new_right;
            (Some(successor), true)
        }
    }
}

fn take_min(mut node: Box<TreeNode>) -> (Option<Box<TreeNode>>, Box<TreeNode>) {
    match node.left.take() {
        None => {
            let right = node.right.take();
            (right, node)
        }
        Some(left) => {
            let (new_left, min) = take_min(left);
            node.left = new_left;
            (Some(node), min)
        }
    }
}

fn inorder(node: Option<&TreeNode>, keys: &mut Vec<i32>) {
    if let Some(node) = node {
        inorder(node.left.as_deref(), keys);
        keys.push(node.key);
        inorder(node.right.as_deref(), keys);
    }
}

fn preorder(node: Option<&TreeNode>, keys: &mut Vec<i32>) {
    if let Some(node) = node {
        keys.push(node.key);
        preorder(node.left.as_deref(), keys);
        preorder(node.right.as_deref(), keys);
    }
}

fn postorder(node: Option<&TreeNode>, keys: &mut Vec<i32>) {
    if let Some(node) = node {
        postorder(node.left.as_deref(), keys);
        postorder(node.right.as_deref(), keys);
        keys.push(node.key);
    }
}

#[cfg(test)]
mod tests {
    use super::BinarySearchTree;

    #[test]
    fn insert_search_and_delete() {
        let mut tree = BinarySearchTree::default();
        for key in [10, 5, 15, 12, 20] {
            tree.insert(key);
        }
        assert!(tree.contains(12));
        assert!(!tree.contains(7));
        assert!(tree.delete(10));
        assert!(!tree.contains(10));
        assert!(tree.contains(5));
        assert!(tree.contains(12));
        assert!(!tree.delete(99));
    }
}
