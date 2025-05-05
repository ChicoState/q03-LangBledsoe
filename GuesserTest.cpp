/**
 * Unit Tests for the class
**/

#include <gtest/gtest.h>
#include "Guesser.h"

class GuesserTest : public ::testing::Test
{
	protected:
		GuesserTest(){} //constructor runs before each test
		virtual ~GuesserTest(){} //destructor cleans up after tests
		virtual void SetUp(){} //sets up before each test (after constructor)
		virtual void TearDown(){} //clean up after each test, (before destructor)
};

// Test that Guesser works with a normal/basic password
TEST(GuesserTest, ConstructorInitializesWithNormalSecret)
{
    Guesser guesser("password");
    ASSERT_EQ(guesser.remaining(), 3);
}

// Test that distance is 0 when strings are the same
TEST(GuesserTest, DistanceIsZeroForIdenticalStrings)
{
    Guesser guesser("secret");
    ASSERT_TRUE(guesser.match("secret"));
}

// Test to make sure Guesser truncation works
TEST(GuesserTest, GuesserTruncatesLongSecret_FullStringFails)
{
    std::string longSecret(100, 'a');
    Guesser guesser(longSecret);
    ASSERT_FALSE(guesser.match(longSecret));
}

// Test that Guesser truncates secrets to 32 characters
TEST(GuesserTest, GuesserTruncatesLongSecret_TruncatedVersionSucceeds)
{
    std::string longSecret(100, 'a');
    Guesser guesser(longSecret);
    ASSERT_TRUE(guesser.match(longSecret.substr(0, 32)));
}

// Test that Guesser works with an empty string
TEST(GuesserTest, GuesserAcceptsEmptyString)
{
    Guesser guesser("");
    ASSERT_TRUE(guesser.match(""));
}

// Test that remaining starts at 3
TEST(GuesserTest, RemainingStartsAtThree)
{
    Guesser guesser("secret");
    ASSERT_EQ(guesser.remaining(), 3);
}

// Test that remaining decreases after an incorrect guess with a small distance
TEST(GuesserTest, RemainingDecreasesAfterIncorrectGuessWithSmallDistance)
{
    Guesser guesser("secret");
    guesser.match("Secret");
    ASSERT_EQ(guesser.remaining(), 2);
}

// Test that remaining doesn't change after a correct guess
TEST(GuesserTest, RemainingUnchangedAfterCorrectGuess)
{
    Guesser guesser("secret");
    guesser.match("secret");
    ASSERT_EQ(guesser.remaining(), 3);
}

// Test that remaining doesn't change after a correct guess with various characters
TEST(GuesserTest, RemainingUnchangedAfterCorrectGuessWithVariousCharacters)
{
    Guesser guesser("$&îAe*O!daR");
    guesser.match("$&îAe*O!daR");
    ASSERT_EQ(guesser.remaining(), 3);
}

/*
  THIS ONE FAILS CURRENT IMPLEMENTATION, but adhears to this logic:
  "If the secret is locked for any other reason, such
  as a big distance in a guess, the count of remaining guesses should still
  count down as usual to hide that the secret has been locked"
*/
// Test that remaining decreases when locked due to brute force
TEST(GuesserTest, RemainingDecreasesWhenLockedDueToBruteForce)
{
    Guesser guesser("secret");
    guesser.match("not_even_close_at_all");
    ASSERT_EQ(guesser.remaining(), 2);
}

// Test that match returns true for an exact match
TEST(GuesserTest, MatchReturnsTrueForExactMatch)
{
    Guesser guesser("secret");
    ASSERT_TRUE(guesser.match("secret"));
}

// Test that match returns false when it doesn't match
TEST(GuesserTest, MatchReturnsFalseForNonMatch)
{
    Guesser guesser("secret");
    ASSERT_FALSE(guesser.match("not_even_close_at_all"));
}

// Test that match returns false when no there are no guesses left
TEST(GuesserTest, MatchReturnsFalseWhenNoGuessesRemain)
{
    Guesser guesser("secret");
    guesser.match("wrong1");
    guesser.match("wrong2");
    guesser.match("wrong3");
    ASSERT_FALSE(guesser.match("secret"));
}

// Test that match locks after a guess with distance bigger than two
TEST(GuesserTest, MatchLocksAfterGuessTooFarFromSecret)
{
    Guesser guesser("secret");
    guesser.match("not_even_close_at_all");
    ASSERT_FALSE(guesser.match("secret"));
}

// Test that match checks for incorrect capitalization
TEST(GuesserTest, MatchIsCaseSensitive_DifferentCaseFails)
{
    Guesser guesser("Secret");
    ASSERT_FALSE(guesser.match("secret"));
}

// Test that match checks for correct capitalization
TEST(GuesserTest, MatchIsCaseSensitive_ExactMatchSucceeds)
{
    Guesser guesser("Secret");
    ASSERT_TRUE(guesser.match("Secret"));
}

// Test that match works with short strings
TEST(GuesserTest, MatchHandlesShorterString)
{
    Guesser guesser("secret");
    ASSERT_FALSE(guesser.match("secre"));
}

// Test that remaining decreases after match with short string
TEST(GuesserTest, RemainingDecreasesAfterShorterStringMatch)
{
    Guesser guesser("secret");
    guesser.match("secre");
    ASSERT_EQ(guesser.remaining(), 2);
}

// Test that match works with long strings
TEST(GuesserTest, MatchHandlesLongerString)
{
    Guesser guesser("secret");
    ASSERT_FALSE(guesser.match("secrets"));
}

// Test that remaining decreases after match with long string
TEST(GuesserTest, RemainingDecreasesAfterLongerStringMatch)
{
    Guesser guesser("secret");
    guesser.match("secrets");
    ASSERT_EQ(guesser.remaining(), 2);
}

// Test that match fails with way different lengths
TEST(GuesserTest, MatchFailsWithVeryDifferentLengthStrings)
{
    Guesser guesser("secret");
    ASSERT_FALSE(guesser.match("secretsecretsecretsecret"));
}

// Test that match locks after attempt with way different lengths
TEST(GuesserTest, MatchLocksAfterVeryDifferentLengthStrings)
{
    Guesser guesser("secret");
    guesser.match("secretsecretsecretsecret");
    ASSERT_FALSE(guesser.match("secret"));
}

// Test that remaining decreases after incorrect guess
TEST(GuesserTest, RemainingDecreasesAfterIncorrectGuess)
{
    Guesser guesser("secret");
    guesser.match("secret!");
    ASSERT_EQ(guesser.remaining(), 2);
}

/* 
  THIS ONE FAILS CURRENT IMPLEMENTATION, but adhears to this logic:
  "However, whenever
  an unlocked secret is guessed with a true match, the guesses remaining
  reset to three (3)"
*/
// Test that remaining resets after successful match
TEST(GuesserTest, RemainingResetsAfterSuccessfulMatch)
{
    Guesser guesser("secret");
    guesser.match("secret!");
    guesser.match("secret");
    ASSERT_EQ(guesser.remaining(), 3);
}

/* 
  THIS ONE FAILS CURRENT IMPLEMENTATION, but adhears to this logic:
  "However, whenever
  an unlocked secret is guessed with a true match, the guesses remaining
  reset to three (3)"
*/
// Test that remaining resets after successful match by matching
TEST(GuesserTest, RemainingResetsAfterSuccessfulMatch_Matching)
{
    Guesser guesser("secret");
    guesser.match("secret!");
    guesser.match("secret");
    guesser.match("secret!");
    guesser.match("secret!");
    ASSERT_TRUE(guesser.match("secret"));
}

// Test distance with one character different
TEST(GuesserTest, DistanceCalculationWithOneCharacterDifferent)
{
    Guesser guesser("secret");
    guesser.match("secret!");
    ASSERT_EQ(guesser.remaining(), 2);
}

// Test distance with two characters different
TEST(GuesserTest, DistanceCalculationWithTwoCharactersDifferent)
{
    Guesser guesser("secret");
    guesser.match("secr11");
    ASSERT_EQ(guesser.remaining(), 2); 
}

// Test distance with two characters less
TEST(GuesserTest, DistanceCalculationWithTwoCharactersLess)
{
    Guesser guesser("secret");
    guesser.match("secr");
    ASSERT_EQ(guesser.remaining(), 2); 
}

// Test distance with two characters more
TEST(GuesserTest, DistanceCalculationWithTwoCharactersMore)
{
    Guesser guesser("secret");
    guesser.match("secret11");
    ASSERT_EQ(guesser.remaining(), 2); 
}

// Test distance with more than two characters different
TEST(GuesserTest, DistanceCalculationWithMoreThanTwoCharactersDifferent)
{
    Guesser guesser("secret");
    guesser.match("sec123");
    ASSERT_FALSE(guesser.match("secret"));
}

// Test distance with empty string
TEST(GuesserTest, DistanceCalculationWithEmptyString)
{
    Guesser guesser("secret");
    guesser.match("");
    ASSERT_FALSE(guesser.match("secret"));
}

// Test distance with very long guess with one character
TEST(GuesserTest, DistanceCalculationWithVeryLongGuess_OneChar)
{
    Guesser guesser("a");
    std::string badGuess(100, 'x');
    guesser.match(badGuess);
    ASSERT_TRUE(guesser.match("a"));
}

// Test distance with very long guess with two characters
TEST(GuesserTest, DistanceCalculationWithVeryLongGuess_TwoChars)
{
    Guesser guesser("aa");
    std::string badGuess(100, 'x');
    guesser.match(badGuess);
    ASSERT_TRUE(guesser.match("aa"));
}

// Test distance with very long guess with three characters
TEST(GuesserTest, DistanceCalculationWithVeryLongGuess_ThreeChars)
{
    Guesser guesser("aaa");
    std::string badGuess(100, 'x');
    guesser.match(badGuess);
    ASSERT_FALSE(guesser.match("aaa"));
}

// Test distance with empty string with one character password
TEST(GuesserTest, DistanceCalculationWithEmptyStringOneCharacter)
{
    Guesser guesser("a");
    guesser.match("");
    ASSERT_TRUE(guesser.match("a"));
}

// Test distance with empty string with two character password
TEST(GuesserTest, DistanceCalculationWithEmptyStringTwoCharacter)
{
    Guesser guesser("aa");
    guesser.match("");
    ASSERT_TRUE(guesser.match("aa"));
}

// Test distance with empty string with three character password
TEST(GuesserTest, DistanceCalculationWithEmptyStringThreeCharacter)
{
    Guesser guesser("aaa");
    guesser.match("");
    ASSERT_FALSE(guesser.match("aaa"));
}

// Test that match works with special characters in secret
TEST(GuesserTest, HandlesSpecialCharactersInSecret_ExactMatch)
{
    Guesser guesser("!@#$%^&*()");
    ASSERT_TRUE(guesser.match("!@#$%^&*()"));
}

// Test that match fails with wrong special characters in guess
TEST(GuesserTest, HandlesSpecialCharactersInSecret_IncorrectGuess)
{
    Guesser guesser("!@#$%^&*()");
    ASSERT_FALSE(guesser.match("!@#$%^&*("));
}

// Test that match works with spaces in secret
TEST(GuesserTest, HandlesSpacesInSecret_ExactMatch)
{
    Guesser guesser("hello world");
    ASSERT_TRUE(guesser.match("hello world"));
}

// Test that match fails when spaces are removed from guess
TEST(GuesserTest, HandlesSpacesInSecret_MissingSpaces)
{
    Guesser guesser("hello world");
    ASSERT_FALSE(guesser.match("helloworld"));
}

// Test that match works with skull emoji 💀💀💀
TEST(GuesserTest, HandlesEmojiCharacters_ExactMatch)
{
    Guesser guesser("hello 💀");
    ASSERT_TRUE(guesser.match("hello 💀"));
}

// Test that match fails with incorrect emoji characters
TEST(GuesserTest, HandlesEmojiCharacters_IncorrectEmoji)
{
    Guesser guesser("hello 💀");
    ASSERT_FALSE(guesser.match("hello 👻"));
}

// Test that match works with characters that aren't ASCII
TEST(GuesserTest, HandlesNonASCIICharacters_ExactMatch)
{
    Guesser guesser("résumé âêîôû");
    ASSERT_TRUE(guesser.match("résumé âêîôû"));
}

// Test that match fails with incorrect non-ASCII characters
TEST(GuesserTest, HandlesNonASCIICharacters_IncorrectChars)
{
    Guesser guesser("résumé âêîôû");
    ASSERT_FALSE(guesser.match("resume aeiou"));
}

// Test that match works with invisible ASCII control characters
TEST(GuesserTest, HandlesInvisibleCharacters_ExactMatch)
{
    std::string secret = "secret" + '\x01';
    Guesser guesser(secret);
    ASSERT_TRUE(guesser.match(secret));
}

// Test that match fails when invisible characters are different
TEST(GuesserTest, HandlesInvisibleCharacters_DifferentInvisibleChars)
{
    std::string secret1 = "secret" + '\x01';
    std::string secret2 = "secret" + '\x02';
    Guesser guesser(secret1);
    ASSERT_FALSE(guesser.match(secret2));
}

// Test that match fails when invisible characters are missing
TEST(GuesserTest, HandlesInvisibleCharacters_MissingInvisibleChars)
{
    std::string secret = "secret" + '\x01';
    Guesser guesser(secret);
    ASSERT_FALSE(guesser.match("secret"));
}

// Make sure that it doesn't just check that passwords are the same length
TEST(GuesserTest, HandlesPasswordsOfDifferentLengths)
{
    Guesser guesser("12345");
    ASSERT_FALSE(guesser.match("54321"));
}

// Test that secret doesn't change for the lifespan of the Guesser object
TEST(GuesserTest, SecretDoesNotChangeForLifespan)
{
    Guesser guesser("secret");
    guesser.match("secret");
    guesser.match("secret");
    guesser.match("secret");
    guesser.match("secret");
    guesser.match("secret");
    guesser.match("secret");
    guesser.match("secret");
    guesser.match("secret");
    guesser.match("secret");
    guesser.match("secret");
    guesser.match("secret");
    guesser.match("secret");
    ASSERT_TRUE(guesser.match("secret"));
}

// Test that secret is locked if there are no remaining guesses
TEST(GuesserTest, SecretIsLockedWhenNoRemainingGuesses)
{
    Guesser guesser("secret");
    guesser.match("secret1");
    guesser.match("secret2");
    guesser.match("secret3");
    ASSERT_FALSE(guesser.match("secret"));
}