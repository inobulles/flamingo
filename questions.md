# Questions

These are questions I've had during development, and that I've found answers to.
I'm keeping track of these so I don't forget why I did something a certain way and am compelled to erroneously change it.

*Q:* Should I really be passing TSNode everywhere instead of TSNode*? It feels like this is prone to me being like "oh, well, the TSNode type is probably hiding a pointer if it's being used as an argument, so I don't have to worry about out-of-scope usage".

*A:* Yes. Tree-sitter passes them as values, so be consistent with that.

*Q:* Do gently detached scopes need to be freed or are they already?

*Q:* Not a question. Fix memory leaks.
