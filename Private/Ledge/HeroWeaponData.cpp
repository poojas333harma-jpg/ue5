// Copyright 2026 Hero Project. All Rights Reserved.

#include "Ledge/HeroWeaponData.h"
#include "Ledge/HeroAttachmentData.h"

float UHeroWeaponData::GetCalculatedWeight() const
{
    float TotalWeight = Stats.ReturnSpeed > 0 ? 2.0f : 5.0f; // Mock base weight for demo
    
    for (const UHeroAttachmentData* Attachment : DefaultAttachments)
    {
        if (Attachment)
        {
            TotalWeight *= Attachment->Modifiers.WeightMultiplier;
        }
    }

    return TotalWeight;
}
