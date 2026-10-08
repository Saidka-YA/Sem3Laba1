use std::collections::VecDeque;

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
