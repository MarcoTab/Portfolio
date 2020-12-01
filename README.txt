EvenOddCipher.java does NOT contain a toString method, this is because the
way that its constructor works makes it possible for the inherited toString
to work perfectly (with respect to the tests). Thus, it didn't make sense
to override.