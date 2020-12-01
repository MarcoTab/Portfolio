import java.util.Arrays;

public class LetterSampleTest {
    public static void main(String[] args) {
        // you are expected to have a public static STOP char. You should be using this instead of '.' as it makes
        // changing '.' to some other char (perhaps '\0') easier (in the case that '.' becomes problematic in the future)
        System.out.println(LetterSample.STOP); // .

        // tests for LetterSample itself.
        LetterSample ls = new LetterSample("alpha", 'b');
        System.out.println(ls); // "alpha" -> b

        System.out.println(ls.getNextLetter()); // b
        System.out.println(ls.getSegment()); // alpha

        // tests for the actual hard part -- toSamples.
        System.out.println(Arrays.toString(LetterSample.toSamples("apple", 2)));
        //["" -> a, "a" -> p, "ap" -> p, "pp" -> l, "pl" -> e, "le" -> .]

        System.out.println(Arrays.toString(LetterSample.toSamples("happened", 4)));
        //["" -> h, "h" -> a, "ha" -> p, "hap" -> p, "happ" -> e, "appe" -> n, "ppen" -> e, "pene" -> d, "ened" -> .]

        System.out.println(Arrays.toString(LetterSample.toSamples("maniac", 1)));
        //["" -> m, "m" -> a, "a" -> n, "n" -> i, "i" -> a, "a" -> c, "c" -> .]

        // you should be filtering out any letter that isn't alphabetical
        System.out.println(Arrays.toString(LetterSample.toSamples("don't", 2)));
        //["" -> d, "d" -> o, "do" -> n, "on" -> t, "nt" -> .]

        System.out.println(Arrays.toString(LetterSample.toSamples("LaTeX", 3)));
        //["" -> l, "l" -> a, "la" -> t, "lat" -> e, "ate" -> x, "tex" -> .]

        System.out.println(Arrays.toString(LetterSample.toSamples("lodge", 0)));
        //["" -> l, "" -> o, "" -> d, "" -> g, "" -> e, "" -> .]

    }
}
/*
.
"alpha" -> b
b
alpha
["" -> a, "a" -> p, "ap" -> p, "pp" -> l, "pl" -> e, "le" -> .]
["" -> h, "h" -> a, "ha" -> p, "hap" -> p, "happ" -> e, "appe" -> n, "ppen" -> e, "pene" -> d, "ened" -> .]
["" -> m, "m" -> a, "a" -> n, "n" -> i, "i" -> a, "a" -> c, "c" -> .]
["" -> d, "d" -> o, "do" -> n, "on" -> t, "nt" -> .]
["" -> l, "l" -> a, "la" -> t, "lat" -> e, "ate" -> x, "tex" -> .]
["" -> l, "" -> o, "" -> d, "" -> g, "" -> e, "" -> .]
 */
