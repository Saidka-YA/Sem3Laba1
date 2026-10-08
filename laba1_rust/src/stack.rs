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

#[cfg(test)]
mod tests {
    use super::*;

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
}
