use std::collections::LinkedList;

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

#[cfg(test)]
mod tests {
    use super::*;

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
}
