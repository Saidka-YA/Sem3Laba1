use laba1_rust::{
    BinarySearchTree, DoubleQueue, DoublyList, DynamicArray, Queue, SinglyList, Stack,
};

fn main() {
    let mut array = DynamicArray::default();
    array.push("one".to_string());
    array.push("two".to_string());
    println!("Массив:");
    array.print();

    let mut single = SinglyList::default();
    single.push_front("first".to_string());
    single.push_back("last".to_string());
    println!("Односвязный список:");
    single.print();

    let mut double = DoublyList::default();
    double.push_back("first".to_string());
    double.push_back("last".to_string());
    println!("Двусвязный список:");
    double.print();

    let mut stack = Stack::default();
    stack.push("first".to_string());
    stack.push("last".to_string());
    println!("Стек: удалён {:?}", stack.pop());

    let mut queue = Queue::default();
    queue.push("first".to_string());
    queue.push("last".to_string());
    println!("Очередь: удалён {:?}", queue.pop());

    let mut double_queue = DoubleQueue::default();
    double_queue.push_back("middle".to_string());
    double_queue.push_front("first".to_string());
    double_queue.push_back("last".to_string());
    println!("Двусвязная очередь:");
    double_queue.print();

    let mut tree = BinarySearchTree::default();
    for key in [10, 5, 15, 12, 20] {
        tree.insert(key);
    }
    println!("Бинарное дерево поиска (симметричный обход):");
    tree.print_inorder();
}
