use std::collections::{LinkedList, VecDeque};

// Динамический массив
#[derive(Default)]
pub struct DynamicArray {
    items: Vec<String>,
}

impl DynamicArray {
    pub fn push(&mut self, value: String) {
        self.items.push(value);
    }
    pub fn insert(&mut self, index: usize, value: String) -> bool {
        if index > self.items.len() {
            return false;
        }
        self.items.insert(index, value);
        true
    }
    pub fn get(&self, index: usize) -> Option<&str> {
        self.items.get(index).map(String::as_str)
    }
    pub fn replace(&mut self, index: usize, value: String) -> bool {
        if index >= self.items.len() {
            return false;
        }
        self.items[index] = value;
        true
    }
    pub fn delete(&mut self, index: usize) -> Option<String> {
        if index >= self.items.len() {
            return None;
        }
        Some(self.items.remove(index))
    }
    pub fn len(&self) -> usize {
        self.items.len()
    }
    pub fn is_empty(&self) -> bool {
        self.items.is_empty()
    }
    pub fn print(&self) {
        println!("{}", self.items.join(" "));
    }
}

// Узел и односвязный список
struct SingleNode {
    value: String,
    next: Option<Box<SingleNode>>,
}

#[derive(Default)]
pub struct SinglyList {
    head: Option<Box<SingleNode>>,
}

impl SinglyList {
    pub fn push_front(&mut self, value: String) {
        let next = self.head.take();
        self.head = Some(Box::new(SingleNode { value, next }));
    }

    pub fn push_back(&mut self, value: String) {
        let mut place = &mut self.head;
        while place.is_some() {
            place = &mut place.as_mut().unwrap().next;
        }
        *place = Some(Box::new(SingleNode { value, next: None }));
    }

    pub fn insert_after(&mut self, anchor: &str, value: String) -> bool {
        let mut node = self.head.as_mut();
        while let Some(current) = node {
            if current.value == anchor {
                let next = current.next.take();
                current.next = Some(Box::new(SingleNode { value, next }));
                return true;
            }
            node = current.next.as_mut();
        }
        false
    }

    pub fn insert_before(&mut self, anchor: &str, value: String) -> bool {
        if self.head.as_ref().is_some_and(|node| node.value == anchor) {
            self.push_front(value);
            return true;
        }
        let mut node = self.head.as_mut();
        while let Some(current) = node {
            if current
                .next
                .as_ref()
                .is_some_and(|next| next.value == anchor)
            {
                let next = current.next.take();
                current.next = Some(Box::new(SingleNode { value, next }));
                return true;
            }
            node = current.next.as_mut();
        }
        false
    }

    pub fn pop_front(&mut self) -> Option<String> {
        self.head.take().map(|mut node| {
            self.head = node.next.take();
            node.value
        })
    }

    pub fn pop_back(&mut self) -> Option<String> {
        if self.head.as_ref()?.next.is_none() {
            return self.pop_front();
        }
        let mut node = self.head.as_mut()?;
        while node.next.as_ref().unwrap().next.is_some() {
            node = node.next.as_mut().unwrap();
        }
        node.next.take().map(|last| last.value)
    }

    pub fn delete_after(&mut self, anchor: &str) -> Option<String> {
        let mut node = self.head.as_mut();
        while let Some(current) = node {
            if current.value == anchor {
                return current.next.take().map(|mut next| {
                    current.next = next.next.take();
                    next.value
                });
            }
            node = current.next.as_mut();
        }
        None
    }

    pub fn delete_before(&mut self, anchor: &str) -> Option<String> {
        if self.head.as_ref()?.value == anchor {
            return None;
        }
        if self.head.as_ref()?.next.as_ref()?.value == anchor {
            return self.pop_front();
        }
        let mut node = self.head.as_mut();
        while let Some(current) = node {
            let next_is_anchor = current
                .next
                .as_ref()
                .and_then(|next| next.next.as_ref())
                .is_some_and(|after| after.value == anchor);
            if next_is_anchor {
                return current.next.take().map(|mut removed| {
                    current.next = removed.next.take();
                    removed.value
                });
            }
            node = current.next.as_mut();
        }
        None
    }

    pub fn delete_value(&mut self, value: &str) -> bool {
        if self.head.as_ref().is_some_and(|node| node.value == value) {
            self.pop_front();
            return true;
        }
        let mut node = self.head.as_mut();
        while let Some(current) = node {
            if current
                .next
                .as_ref()
                .is_some_and(|next| next.value == value)
            {
                let mut removed = current.next.take().unwrap();
                current.next = removed.next.take();
                return true;
            }
            node = current.next.as_mut();
        }
        false
    }

    pub fn contains(&self, value: &str) -> bool {
        let mut node = self.head.as_deref();
        while let Some(current) = node {
            if current.value == value {
                return true;
            }
            node = current.next.as_deref();
        }
        false
    }

    pub fn print(&self) {
        let mut values = Vec::new();
        let mut node = self.head.as_deref();
        while let Some(current) = node {
            values.push(current.value.as_str());
            node = current.next.as_deref();
        }
        println!("{}", values.join(" "));
    }
}

// Двусвязный список из стандартной библиотеки Rust
#[derive(Default)]
pub struct DoublyList {
    items: LinkedList<String>,
}

impl DoublyList {
    pub fn push_front(&mut self, value: String) {
        self.items.push_front(value);
    }
    pub fn push_back(&mut self, value: String) {
        self.items.push_back(value);
    }
    pub fn pop_front(&mut self) -> Option<String> {
        self.items.pop_front()
    }
    pub fn pop_back(&mut self) -> Option<String> {
        self.items.pop_back()
    }
    pub fn contains(&self, value: &str) -> bool {
        self.items.iter().any(|item| item == value)
    }

    pub fn insert_before(&mut self, anchor: &str, value: String) -> bool {
        let Some(index) = self.items.iter().position(|item| item == anchor) else {
            return false;
        };
        let mut right = self.items.split_off(index);
        self.items.push_back(value);
        self.items.append(&mut right);
        true
    }

    pub fn insert_after(&mut self, anchor: &str, value: String) -> bool {
        let Some(index) = self.items.iter().position(|item| item == anchor) else {
            return false;
        };
        let mut right = self.items.split_off(index + 1);
        self.items.push_back(value);
        self.items.append(&mut right);
        true
    }

    pub fn delete_value(&mut self, value: &str) -> bool {
        let Some(index) = self.items.iter().position(|item| item == value) else {
            return false;
        };
        self.remove_at(index).is_some()
    }

    pub fn delete_before(&mut self, anchor: &str) -> Option<String> {
        let index = self.items.iter().position(|item| item == anchor)?;
        if index == 0 {
            return None;
        }
        self.remove_at(index - 1)
    }

    pub fn delete_after(&mut self, anchor: &str) -> Option<String> {
        let index = self.items.iter().position(|item| item == anchor)?;
        self.remove_at(index + 1)
    }

    fn remove_at(&mut self, index: usize) -> Option<String> {
        if index >= self.items.len() {
            return None;
        }
        let mut right = self.items.split_off(index);
        let removed = right.pop_front();
        self.items.append(&mut right);
        removed
    }

    pub fn print(&self) {
        println!(
            "{}",
            self.items.iter().cloned().collect::<Vec<_>>().join(" ")
        );
    }
}

// Стек: последний добавленный элемент удаляется первым
#[derive(Default)]
pub struct Stack {
    items: Vec<String>,
}

impl Stack {
    pub fn push(&mut self, value: String) {
        self.items.push(value);
    }
    pub fn pop(&mut self) -> Option<String> {
        self.items.pop()
    }
    pub fn peek(&self) -> Option<&str> {
        self.items.last().map(String::as_str)
    }
    pub fn is_empty(&self) -> bool {
        self.items.is_empty()
    }
    pub fn print(&self) {
        println!(
            "{}",
            self.items
                .iter()
                .rev()
                .cloned()
                .collect::<Vec<_>>()
                .join(" ")
        );
    }
}

// Очередь: первым удаляется элемент, добавленный раньше остальных
#[derive(Default)]
pub struct Queue {
    items: VecDeque<String>,
}

impl Queue {
    pub fn push(&mut self, value: String) {
        self.items.push_back(value);
    }
    pub fn pop(&mut self) -> Option<String> {
        self.items.pop_front()
    }
    pub fn front(&self) -> Option<&str> {
        self.items.front().map(String::as_str)
    }
    pub fn len(&self) -> usize {
        self.items.len()
    }
    pub fn is_empty(&self) -> bool {
        self.items.is_empty()
    }
    pub fn print(&self) {
        println!(
            "{}",
            self.items.iter().cloned().collect::<Vec<_>>().join(" ")
        );
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn dynamic_array_operations() {
        let mut array = DynamicArray::default();
        array.push("A".into());
        array.push("C".into());
        assert!(array.insert(1, "B".into()));
        assert_eq!(array.get(1), Some("B"));
        assert!(array.replace(1, "b".into()));
        assert_eq!(array.delete(1).as_deref(), Some("b"));
        assert_eq!(array.len(), 2);
        assert!(!array.insert(5, "X".into()));
        assert!(!array.is_empty());
    }

    #[test]
    fn singly_linked_list_operations() {
        let mut list = SinglyList::default();
        list.push_front("B".into());
        list.push_front("A".into());
        list.push_back("D".into());
        assert!(list.insert_after("B", "C".into()));
        assert!(list.insert_before("D", "E".into()));
        assert!(list.contains("C"));
        assert_eq!(list.delete_after("B").as_deref(), Some("C"));
        assert_eq!(list.delete_before("D").as_deref(), Some("E"));
        assert!(list.delete_value("B"));
        assert_eq!(list.pop_back().as_deref(), Some("D"));
        assert_eq!(list.pop_front().as_deref(), Some("A"));
        assert_eq!(list.pop_front(), None);
    }

    #[test]
    fn doubly_linked_list_operations() {
        let mut list = DoublyList::default();
        list.push_front("B".into());
        list.push_front("A".into());
        list.push_back("D".into());
        assert!(list.insert_after("B", "C".into()));
        assert!(list.insert_before("D", "E".into()));
        assert!(list.contains("C"));
        assert_eq!(list.delete_after("B").as_deref(), Some("C"));
        assert_eq!(list.delete_before("D").as_deref(), Some("E"));
        assert!(list.delete_value("B"));
        assert_eq!(list.pop_back().as_deref(), Some("D"));
        assert_eq!(list.pop_front().as_deref(), Some("A"));
        assert_eq!(list.pop_front(), None);
    }

    #[test]
    fn stack_is_last_in_first_out() {
        let mut stack = Stack::default();
        assert!(stack.is_empty());
        stack.push("first".into());
        stack.push("second".into());
        assert_eq!(stack.peek(), Some("second"));
        assert_eq!(stack.pop().as_deref(), Some("second"));
        assert_eq!(stack.pop().as_deref(), Some("first"));
        assert_eq!(stack.pop(), None);
    }

    #[test]
    fn queue_is_first_in_first_out() {
        let mut queue = Queue::default();
        assert!(queue.is_empty());
        queue.push("first".into());
        queue.push("second".into());
        assert_eq!(queue.front(), Some("first"));
        assert_eq!(queue.len(), 2);
        assert_eq!(queue.pop().as_deref(), Some("first"));
        assert_eq!(queue.pop().as_deref(), Some("second"));
        assert_eq!(queue.pop(), None);
    }
}
