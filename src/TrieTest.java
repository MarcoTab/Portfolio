public class TrieTest {
    public static void main(String[] args) {
        // This class is a bit harder to test. There's a lot of how it behaves _internally_
        // that we care about, but which is hard to really expose for testing. I don't think
        // I have an easy way to help you test that unfortunately.


        Trie<Integer> numbers = new Trie<>();
        System.out.println(numbers.get("one") == null); // true

        numbers.set("one", 1);
        System.out.println(numbers.get("one")); // 1
        System.out.println(numbers.getRoot().getTreeSize()); // 4
        // (remember the root represents "", then it should have "o" as a child, then "on" etc.)

        System.out.println(numbers.get("two") == null); // true

        numbers.set("two", 2);
        System.out.println(numbers.get("two")); // 2
        System.out.println(numbers.getRoot().getTreeSize()); // 7

        System.out.println(numbers.get("ten") == null); // true

        numbers.set("ten", 10);
        System.out.println(numbers.get("ten")); // 10
        System.out.println(numbers.getRoot().getTreeSize()); // 9


    }
}
/*
true
1
4
true
2
7
true
10
9
 */