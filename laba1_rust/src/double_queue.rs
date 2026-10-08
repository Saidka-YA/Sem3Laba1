use std::collections::LinkedList;

// Двусвязная очередь: добавление и удаление с обоих концов.
#[derive(Default)]
pub struct DoubleQueue {
    items: LinkedList<String>,
}

impl DoubleQueue {
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

    pub fn front(&self) -> Option<&str> {
        self.items.front().map(String::as_str)
    }

    pub fn back(&self) -> Option<&str> {
        self.items.back().map(String::as_str)
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
    use super::DoubleQueue;

    #[test]
    fn operations_at_both_ends() {
        let mut queue = DoubleQueue::default();
        queue.push_back("B".into());
        queue.push_front("A".into());
        queue.push_back("C".into());
        assert_eq!(queue.front(), Some("A"));
        assert_eq!(queue.back(), Some("C"));
        assert_eq!(queue.pop_back().as_deref(), Some("C"));
        assert_eq!(queue.pop_front().as_deref(), Some("A"));
        assert_eq!(queue.pop_front().as_deref(), Some("B"));
        assert!(queue.is_empty());
    }
}
