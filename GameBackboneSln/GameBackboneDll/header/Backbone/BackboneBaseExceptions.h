#pragma once

#include <exception>

namespace GB {
	namespace Error {

		/// <summary>
		/// Exception thrown when a GameRegion attempts to disassociate a
		/// non-associated GameRegion.
		/// </summary>
		/// <seealso cref="std::exception" />
		class GameRegion_BadDissociation : public std::exception
		{
		public:
			virtual const char* what() const override {
				return "Attempting to remove a non-associated object is not allowed";
			}
		};

		/// <summary>
		/// Exception thrown when an AnimatedSprite tries to run an empty animation.
		/// </summary>
		/// <seealso cref="std::exception" />
		class AnimatedSprite_EmptyAnimation : public std::exception
		{
		public:
			virtual const char* what() const override {
				return "Cannot run an empty animation.";
			}
		};

		/// <summary>
		/// Exception thrown when an FileReader fails to open a file.
		/// </summary>
		/// <seealso cref="std::exception" />
		class FileReader_BadFile : public std::exception
		{
		public:
			virtual const char* what() const override {
				return "Cannot open the file.";
			}
		};
		
		/// <summary>
		/// Exception thrown when input vectors for navigation functions are not of compatible sizes.
		/// </summary>
		/// <seealso cref="std::exception" />
		class NavigationTools_MismatchedNavigationSizes : public std::exception {
		public:
			virtual const char* what() const override {
				return "The passed arrays are not of compatible sizes.";
			}
		};
	}
}

