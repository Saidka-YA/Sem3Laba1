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
}
