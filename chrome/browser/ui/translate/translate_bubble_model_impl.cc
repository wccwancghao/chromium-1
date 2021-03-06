// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/translate/translate_bubble_model_impl.h"

#include "chrome/browser/translate/translate_tab_helper.h"
#include "chrome/browser/translate/translate_ui_delegate.h"
#include "components/translate/core/browser/language_state.h"

TranslateBubbleModelImpl::TranslateBubbleModelImpl(
    TranslateTabHelper::TranslateStep step,
    scoped_ptr<TranslateUIDelegate> ui_delegate)
    : ui_delegate_(ui_delegate.Pass()),
      view_state_transition_(TranslateStepToViewState(step)) {}

TranslateBubbleModelImpl::~TranslateBubbleModelImpl() {
}

// static
TranslateBubbleModel::ViewState
TranslateBubbleModelImpl::TranslateStepToViewState(
    TranslateTabHelper::TranslateStep step) {
  switch (step) {
    case TranslateTabHelper::BEFORE_TRANSLATE:
      return TranslateBubbleModel::VIEW_STATE_BEFORE_TRANSLATE;
    case TranslateTabHelper::TRANSLATING:
      return TranslateBubbleModel::VIEW_STATE_TRANSLATING;
    case TranslateTabHelper::AFTER_TRANSLATE:
      return TranslateBubbleModel::VIEW_STATE_AFTER_TRANSLATE;
    case TranslateTabHelper::TRANSLATE_ERROR:
      return TranslateBubbleModel::VIEW_STATE_ERROR;
  }

  NOTREACHED();
  return TranslateBubbleModel::VIEW_STATE_ERROR;
}

TranslateBubbleModel::ViewState TranslateBubbleModelImpl::GetViewState() const {
  return view_state_transition_.view_state();
}

void TranslateBubbleModelImpl::SetViewState(
    TranslateBubbleModel::ViewState view_state) {
  view_state_transition_.SetViewState(view_state);
}

void TranslateBubbleModelImpl::ShowError(TranslateErrors::Type error_type) {
  ui_delegate_->OnErrorShown(error_type);
}

void TranslateBubbleModelImpl::GoBackFromAdvanced() {
  view_state_transition_.GoBackFromAdvanced();
}

int TranslateBubbleModelImpl::GetNumberOfLanguages() const {
  return ui_delegate_->GetNumberOfLanguages();
}

base::string16 TranslateBubbleModelImpl::GetLanguageNameAt(int index) const {
  return ui_delegate_->GetLanguageNameAt(index);
}

int TranslateBubbleModelImpl::GetOriginalLanguageIndex() const {
  return ui_delegate_->GetOriginalLanguageIndex();
}

void TranslateBubbleModelImpl::UpdateOriginalLanguageIndex(int index) {
  ui_delegate_->UpdateOriginalLanguageIndex(index);
}

int TranslateBubbleModelImpl::GetTargetLanguageIndex() const {
  return ui_delegate_->GetTargetLanguageIndex();
}

void TranslateBubbleModelImpl::UpdateTargetLanguageIndex(int index) {
  ui_delegate_->UpdateTargetLanguageIndex(index);
}

void TranslateBubbleModelImpl::SetNeverTranslateLanguage(bool value) {
  ui_delegate_->SetLanguageBlocked(value);
}

void TranslateBubbleModelImpl::SetNeverTranslateSite(bool value) {
  ui_delegate_->SetSiteBlacklist(value);
}

bool TranslateBubbleModelImpl::ShouldAlwaysTranslate() const {
  return ui_delegate_->ShouldAlwaysTranslate();
}

void TranslateBubbleModelImpl::SetAlwaysTranslate(bool value) {
  ui_delegate_->SetAlwaysTranslate(value);
}

void TranslateBubbleModelImpl::Translate() {
  ui_delegate_->Translate();
}

void TranslateBubbleModelImpl::RevertTranslation() {
  ui_delegate_->RevertTranslation();
}

void TranslateBubbleModelImpl::TranslationDeclined(bool explicitly_closed) {
  ui_delegate_->TranslationDeclined(explicitly_closed);
}

bool TranslateBubbleModelImpl::IsPageTranslatedInCurrentLanguages() const {
  content::WebContents* web_contents = ui_delegate_->web_contents();
  TranslateTabHelper* translate_tab_helper =
      TranslateTabHelper::FromWebContents(web_contents);
  LanguageState& language_state = translate_tab_helper->GetLanguageState();
  return ui_delegate_->GetOriginalLanguageCode() ==
      language_state.original_language() &&
      ui_delegate_->GetTargetLanguageCode() ==
      language_state.current_language();
}
