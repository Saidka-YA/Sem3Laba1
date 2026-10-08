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

#[cfg(test)]
mod tests {
    use super::*;

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
}
