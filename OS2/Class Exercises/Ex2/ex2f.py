#prints the sentence's words
print("Enter Your sentence:")
sentence = [str(x)for x in input().split()]
for x in range(len(sentence)):
    print(sentence[x])