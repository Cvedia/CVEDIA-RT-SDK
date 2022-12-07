// imgui-notify by patrickcjk
// https://github.com/patrickcjk/imgui-notify

#pragma once

#include <cstdarg>
#include <vector>
#include <string>
#include "font_awesome_5.h"
#include "fa_solid_900.h"

#ifndef _WIN32
#include <sys/sysinfo.h>
#endif

#define NOTIFY_PADDING_X				20.f		// Bottom-left X padding
#define NOTIFY_PADDING_Y				20.f		// Bottom-left Y padding
#define NOTIFY_PADDING_MESSAGE_Y		10.f		// Padding Y between each message
#define NOTIFY_FADE_IN_OUT_TIME			150ULL		// Fade in and out duration
#define NOTIFY_DEFAULT_DISMISS			3000		// Auto dismiss after X ms (default, applied only of no data provided in constructors)
#define NOTIFY_OPACITY					1.0f		// 0-1 Toast opacity
#define NOTIFY_TOAST_FLAGS				ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav/* | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing*/
// Comment out if you don't want any separator between title and content
#define NOTIFY_USE_SEPARATOR

#define NOTIFY_INLINE					inline
#define NOTIFY_NULL_OR_EMPTY(str)		(str.length() <= 0)

typedef int ImGuiToastType;
typedef int ImGuiToastPhase;
typedef int ImGuiToastPos;

enum ImGuiToastType_
{
	ImGuiToastType_None,
	ImGuiToastType_Success,
	ImGuiToastType_Warning,
	ImGuiToastType_Error,
	ImGuiToastType_Info,
	ImGuiToastType_COUNT
};

enum ImGuiToastPhase_
{
	ImGuiToastPhase_FadeIn,
	ImGuiToastPhase_Wait,
	ImGuiToastPhase_FadeOut,
	ImGuiToastPhase_Expired,
	ImGuiToastPhase_COUNT
};

enum ImGuiToastPos_
{
	ImGuiToastPos_TopLeft,
	ImGuiToastPos_TopCenter,
	ImGuiToastPos_TopRight,
	ImGuiToastPos_BottomLeft,
	ImGuiToastPos_BottomCenter,
	ImGuiToastPos_BottomRight,
	ImGuiToastPos_Center,
	ImGuiToastPos_COUNT
};

uint64_t getTicks() {
#ifdef _WIN32
	return GetTickCount64();
#else
	uint64_t uptime(0u);
	struct sysinfo x;
	if (sysinfo(&x) == 0)
	{
		uptime = static_cast<unsigned long long>(x.uptime)*1000ULL;
	}
	return uptime;
#endif
}


std::string variable_str_format(std::string format, va_list args) {
	std::string result;
	const char * c_format = format.c_str();
	va_list args_cpy;
	va_copy(args_cpy, args);

	int len = vsnprintf(nullptr, 0, c_format, args);
	if (len < 0) {
		va_end(args);
		return format;
	}
	if (len > 0) {
    	result.resize(len);
    	vsnprintf(&result[0], len+1, c_format, args); // or result.data() in C++17 and later...
	}
	va_end(args_cpy);
	return result;
}


class ImGuiToast
{
private:
	ImGuiToastType	type = ImGuiToastType_None;
	std::string		title;
	std::string		content;
	int				dismiss_time = NOTIFY_DEFAULT_DISMISS;
	uint64_t 		creation_time = 0ULL;

public:

	void set_title(const std::string& title_) {
		 this->title = title_;
	}

	void set_content(const std::string& content_) { 
		this->content = content_;
	}

	void set_type(const ImGuiToastType& type_)
	{ 
		IM_ASSERT(type_ < ImGuiToastType_COUNT); this->type = type_;
	};

public:
	// Getters

	NOTIFY_INLINE auto get_title() -> std::string { return this->title; };

	NOTIFY_INLINE auto get_default_title() -> std::string
	{
		if (this->title.length() <= 0)
		{
			switch (this->type)
			{
			case ImGuiToastType_None:
				return NULL;
			case ImGuiToastType_Success:
				return "Success";
			case ImGuiToastType_Warning:
				return "Warning";
			case ImGuiToastType_Error:
				return "Error";
			case ImGuiToastType_Info:
				return "Info";
			}
		}

		return this->title;
	};

	NOTIFY_INLINE auto get_type() -> const ImGuiToastType& {
		return this->type;
	};

	NOTIFY_INLINE auto get_color() -> ImVec4
	{
		switch (this->type)
		{
		case ImGuiToastType_None:
			return { 255, 255, 255, 255 }; // White
		case ImGuiToastType_Success:
			return { 0, 255, 0, 255 }; // Green
		case ImGuiToastType_Warning:
			return { 255, 255, 0, 255 }; // Yellow
		case ImGuiToastType_Error:
			return { 255, 0, 0, 255 }; // Error
		case ImGuiToastType_Info:
			return { 0, 157, 255, 255 }; // Blue
		}

		return { 255, 255, 255, 255 };
	}

	NOTIFY_INLINE auto get_icon() -> std::string
	{
		switch (this->type)
		{
		case ImGuiToastType_None:
			return "";
		case ImGuiToastType_Success:
			return ICON_FA_CHECK_CIRCLE;
		case ImGuiToastType_Warning:
			return ICON_FA_EXCLAMATION_TRIANGLE;
		case ImGuiToastType_Error:
			return ICON_FA_TIMES_CIRCLE;
		case ImGuiToastType_Info:
			return ICON_FA_INFO_CIRCLE;
		}
		return "";
	}

	NOTIFY_INLINE auto get_content() -> std::string { return this->content; };

	NOTIFY_INLINE auto get_elapsed_time() { return getTicks() - this->creation_time; }

	NOTIFY_INLINE auto get_phase() -> ImGuiToastPhase
	{
		const auto elapsed = get_elapsed_time();

		if (elapsed > NOTIFY_FADE_IN_OUT_TIME + this->dismiss_time + NOTIFY_FADE_IN_OUT_TIME)
		{
			return ImGuiToastPhase_Expired;
		}
		else if (elapsed > NOTIFY_FADE_IN_OUT_TIME + this->dismiss_time)
		{
			return ImGuiToastPhase_FadeOut;
		}
		else if (elapsed > NOTIFY_FADE_IN_OUT_TIME)
		{
			return ImGuiToastPhase_Wait;
		}
		else
		{
			return ImGuiToastPhase_FadeIn;
		}
	}

	NOTIFY_INLINE auto get_fade_percent() -> float
	{
		const auto phase = get_phase();
		const auto elapsed = get_elapsed_time();

		if (phase == ImGuiToastPhase_FadeIn)
		{
			return ((float)elapsed / (float)NOTIFY_FADE_IN_OUT_TIME) * NOTIFY_OPACITY;
		}
		else if (phase == ImGuiToastPhase_FadeOut)
		{
			return (1.f - (((float)elapsed - (float)NOTIFY_FADE_IN_OUT_TIME - (float)this->dismiss_time) / (float)NOTIFY_FADE_IN_OUT_TIME)) * NOTIFY_OPACITY;
		}

		return 1.f * NOTIFY_OPACITY;
	}

public:
	// Constructors

	ImGuiToast(ImGuiToastType type, int dismiss_time = NOTIFY_DEFAULT_DISMISS)
	{
		IM_ASSERT(type < ImGuiToastType_COUNT);

		this->type = type;
		this->dismiss_time = dismiss_time;
		this->creation_time = getTicks();

		this->title = "";
		this->content = "";
	}
};

namespace ImGui
{
	std::vector<ImGuiToast> notifications;

	/// <summary>
	/// Insert a new toast in the list
	/// </summary>
	void InsertNotification(const ImGuiToast& toast)
	{
		notifications.push_back(toast);
	}

	/// <summary>
	/// Remove a toast from the list by its index
	/// </summary>
	/// <param name="index">index of the toast to remove</param>
	void RemoveNotification(int index)
	{
		notifications.erase(notifications.begin() + index);
	}

	/// <summary>
	/// Render toasts, call at the end of your rendering!
	/// </summary>
	void RenderNotifications()
	{
		const auto vp_size = GetMainViewport()->Size;

		float height = 0.f;

		for (size_t i = 0; i < (size_t)notifications.size(); i++)
		{
			auto* current_toast = &notifications[i];

			// Remove toast if expired
			if (current_toast->get_phase() == ImGuiToastPhase_Expired)
			{
				RemoveNotification(static_cast<int>(i));
				continue;
			}

			// Get icon, title and other data
			const auto icon = current_toast->get_icon();
			const auto title = current_toast->get_title();
			const auto content = current_toast->get_content();
			const auto default_title = current_toast->get_default_title();
			const auto opacity = current_toast->get_fade_percent(); // Get opacity based of the current phase

			// Window rendering
			auto text_color = current_toast->get_color();
			text_color.w = opacity;

			// Generate new unique name for this toast
			std::string window_name;
			window_name = "##TOAST" + std::to_string(i);

			SetNextWindowBgAlpha(opacity);
			SetNextWindowPos(ImVec2(vp_size.x - NOTIFY_PADDING_X, vp_size.y - NOTIFY_PADDING_Y - height), ImGuiCond_Always, ImVec2(1.0f, 1.0f));
			Begin(window_name.c_str(), NULL, NOTIFY_TOAST_FLAGS);

			// Here we render the toast content
			{
				PushTextWrapPos(vp_size.x / 3.f); // We want to support multi-line text, this will wrap the text after 1/3 of the screen width

				bool was_title_rendered = false;

				// If an icon is set
				if (!NOTIFY_NULL_OR_EMPTY(icon))
				{
					TextColored(text_color, "%s", icon.c_str());
					was_title_rendered = true;
				}

				// If a title is set
				if (!NOTIFY_NULL_OR_EMPTY(title))
				{
					// If a title and an icon is set, we want to render on same line
					if (!NOTIFY_NULL_OR_EMPTY(icon))
						SameLine();

					Text("%s", title.c_str()); // Render title text
					was_title_rendered = true;
				}
				else if (!NOTIFY_NULL_OR_EMPTY(default_title))
				{
					if (!NOTIFY_NULL_OR_EMPTY(icon))
						SameLine();

					Text("%s", default_title.c_str()); // Render default title text (ImGuiToastType_Success -> "Success", etc...)
					was_title_rendered = true;
				}

				// In case ANYTHING was rendered in the top, we want to add a small padding so the text (or icon) looks centered vertically
				if (was_title_rendered && !NOTIFY_NULL_OR_EMPTY(content))
				{
					SetCursorPosY(GetCursorPosY() + 5.f); // Must be a better way to do this!!!!
				}

				// If a content is set
				if (!NOTIFY_NULL_OR_EMPTY(content))
				{
					if (was_title_rendered)
					{
#ifdef NOTIFY_USE_SEPARATOR
						Separator();
#endif
					}

					Text("%s", content.c_str()); // Render content text
				}

				PopTextWrapPos();
			}

			// Save height for next toasts
			height += GetWindowHeight() + NOTIFY_PADDING_MESSAGE_Y;

			// End
			End();
		}
	}

	/// <summary>
	/// Adds font-awesome font, must be called ONCE on initialization
	/// <param name="FontDataOwnedByAtlas">Fonts are loaded from read-only memory, should be set to false!</param>
	/// </summary>
	void MergeIconsWithLatestFont(float font_size, bool FontDataOwnedByAtlas = false)
	{
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.FontDataOwnedByAtlas = FontDataOwnedByAtlas;

		GetIO().Fonts->AddFontFromMemoryTTF((void*)fa_solid_900, sizeof(fa_solid_900), font_size, &icons_config, icons_ranges);
	}
}
