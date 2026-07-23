/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "DruidAiObjectContext.h"

#include "BalanceDruidStrategy.h"
#include "BearDruidStrategy.h"
#include "CatDruidStrategy.h"
#include "DruidActions.h"
#include "DruidBearActions.h"
#include "DruidCatActions.h"
#include "DruidShapeshiftActions.h"
#include "DruidTriggers.h"
#include "GenericDruidNonCombatStrategy.h"
#include "GenericDruidStrategy.h"
#include "RestoDruidStrategy.h"
#include "DruidPullStrategy.h"

class DruidStrategyFactoryInternal : public NamedObjectContext<Strategy>
{
public:
    DruidStrategyFactoryInternal()
    {
        creators["nc"] = &DruidStrategyFactoryInternal::nc;
        creators["pull"] = &DruidStrategyFactoryInternal::pull;
        creators["aoe"] = &DruidStrategyFactoryInternal::aoe;
        creators["cure"] = &DruidStrategyFactoryInternal::cure;
        creators["buff"] = &DruidStrategyFactoryInternal::buff;
        creators["boost"] = &DruidStrategyFactoryInternal::boost;
        creators["cc"] = &DruidStrategyFactoryInternal::cc;
        creators["healer dps"] = &DruidStrategyFactoryInternal::healer_dps;
        creators["offheal"] = &DruidStrategyFactoryInternal::offheal;
        creators["blanketing"] = &DruidStrategyFactoryInternal::blanketing;
        creators["tranquility"] = &DruidStrategyFactoryInternal::tranquility;
        creators["feral charge"] = &DruidStrategyFactoryInternal::feral_charge;
    }

private:
    static Strategy* nc(PlayerbotAI* botAI) { return new GenericDruidNonCombatStrategy(botAI); }
    static Strategy* pull(PlayerbotAI* botAI) { return new DruidPullStrategy(botAI); }
    static Strategy* aoe(PlayerbotAI* botAI) { return new DruidAoeStrategy(botAI); }
    static Strategy* cure(PlayerbotAI* botAI) { return new DruidCureStrategy(botAI); }
    static Strategy* buff(PlayerbotAI* botAI) { return new GenericDruidBuffStrategy(botAI); }
    static Strategy* boost(PlayerbotAI* botAI) { return new DruidBoostStrategy(botAI); }
    static Strategy* cc(PlayerbotAI* botAI) { return new DruidCcStrategy(botAI); }
    static Strategy* healer_dps(PlayerbotAI* botAI) { return new DruidHealerDpsStrategy(botAI); }
    static Strategy* offheal(PlayerbotAI* botAI) { return new CatOffhealStrategy(botAI); }
    static Strategy* blanketing(PlayerbotAI* botAI) { return new DruidBlanketStrategy(botAI); }
    static Strategy* tranquility(PlayerbotAI* botAI) { return new DruidTranquilityStrategy(botAI); }
    static Strategy* feral_charge(PlayerbotAI* botAI) { return new FeralChargeDruidStrategy(botAI); }
};

class DruidDruidStrategyFactoryInternal : public NamedObjectContext<Strategy>
{
public:
    DruidDruidStrategyFactoryInternal() : NamedObjectContext<Strategy>(false, true)
    {
        creators["bear"] = &DruidDruidStrategyFactoryInternal::bear;
        creators["tank"] = &DruidDruidStrategyFactoryInternal::bear;
        creators["cat"] = &DruidDruidStrategyFactoryInternal::cat;
        creators["balance"] = &DruidDruidStrategyFactoryInternal::balance;
        creators["dps"] = &DruidDruidStrategyFactoryInternal::cat;
        creators["resto"] = &DruidDruidStrategyFactoryInternal::heal;
    }

private:
    static Strategy* bear(PlayerbotAI* botAI) { return new BearDruidStrategy(botAI); }
    static Strategy* cat(PlayerbotAI* botAI) { return new CatDruidStrategy(botAI); }
    static Strategy* balance(PlayerbotAI* botAI) { return new BalanceDruidStrategy(botAI); }
    static Strategy* heal(PlayerbotAI* botAI) { return new RestoDruidStrategy(botAI); }
};

class DruidTriggerFactoryInternal : public NamedObjectContext<Trigger>
{
public:
    DruidTriggerFactoryInternal()
    {
        creators["clearcasting"] = &DruidTriggerFactoryInternal::clearcasting;
        creators["thorns"] = &DruidTriggerFactoryInternal::thorns;
        creators["thorns on party"] = &DruidTriggerFactoryInternal::thorns_on_party;
        creators["thorns on main tank"] = &DruidTriggerFactoryInternal::thorns_on_main_tank;
        creators["bash"] = &DruidTriggerFactoryInternal::bash;
        creators["faerie fire (feral)"] = &DruidTriggerFactoryInternal::faerie_fire_feral;
        creators["faerie fire"] = &DruidTriggerFactoryInternal::faerie_fire;
        creators["insect swarm"] = &DruidTriggerFactoryInternal::insect_swarm;
        creators["insect swarm on attacker"] = &DruidTriggerFactoryInternal::insect_swarm_on_attacker;
        creators["moonfire"] = &DruidTriggerFactoryInternal::moonfire;
        creators["moonfire on attacker"] = &DruidTriggerFactoryInternal::moonfire_on_attacker;
        creators["nature's grasp"] = &DruidTriggerFactoryInternal::natures_grasp;
        creators["berserk"] = &DruidTriggerFactoryInternal::berserk;
        creators["berserk active"] = &DruidTriggerFactoryInternal::berserk_active;
        creators["savage roar"] = &DruidTriggerFactoryInternal::savage_roar;
        creators["rake"] = &DruidTriggerFactoryInternal::rake;
        creators["mark of the wild"] = &DruidTriggerFactoryInternal::mark_of_the_wild;
        creators["mark of the wild on party"] = &DruidTriggerFactoryInternal::mark_of_the_wild_on_party;
        creators["cure poison"] = &DruidTriggerFactoryInternal::cure_poison;
        creators["party member cure poison"] = &DruidTriggerFactoryInternal::party_member_cure_poison;
        creators["entangling roots"] = &DruidTriggerFactoryInternal::entangling_roots;
        creators["entangling roots kite"] = &DruidTriggerFactoryInternal::entangling_roots_kite;
        creators["hibernate"] = &DruidTriggerFactoryInternal::hibernate;
        creators["bear form"] = &DruidTriggerFactoryInternal::bear_form;
        creators["cat form"] = &DruidTriggerFactoryInternal::cat_form;
        creators["tree form"] = &DruidTriggerFactoryInternal::tree_form;
        creators["eclipse (solar)"] = &DruidTriggerFactoryInternal::eclipse_solar;
        creators["eclipse (lunar)"] = &DruidTriggerFactoryInternal::eclipse_lunar;
        creators["bash on enemy healer"] = &DruidTriggerFactoryInternal::bash_on_enemy_healer;
        creators["nature's swiftness"] = &DruidTriggerFactoryInternal::natures_swiftness;
        creators["nature's swiftness active"] = &DruidTriggerFactoryInternal::natures_swiftness_active;
        creators["party member remove curse"] = &DruidTriggerFactoryInternal::party_member_remove_curse;
        creators["mangle (bear)"] = &DruidTriggerFactoryInternal::mangle_bear_trigger;
        creators["lacerate"] = &DruidTriggerFactoryInternal::lacerate_trigger;
        creators["demoralizing roar"] = &DruidTriggerFactoryInternal::demoralize_roar;
        creators["mangle (cat)"] = &DruidTriggerFactoryInternal::mangle_cat;
        creators["ferocious bite time"] = &DruidTriggerFactoryInternal::ferocious_bite_time;
        creators["ferocious bite execute"] = &DruidTriggerFactoryInternal::ferocious_bite_execute;
        creators["hurricane channel check"] = &DruidTriggerFactoryInternal::hurricane_channel_check;
        creators["no healer dps strategy"] = &DruidTriggerFactoryInternal::no_healer_dps_strategy;
        creators["starfall"] = &DruidTriggerFactoryInternal::starfall;
        creators["force of nature"] = &DruidTriggerFactoryInternal::force_of_nature;
        creators["cyclone"] = &DruidTriggerFactoryInternal::cyclone;
        creators["predator's swiftness"] = &DruidTriggerFactoryInternal::predators_swiftness;
        creators["predator's swiftness and cyclone"] = &DruidTriggerFactoryInternal::predators_swiftness_and_cyclone;
        creators["predator's swiftness and hibernate"] = &DruidTriggerFactoryInternal::predators_swiftness_and_hibernate;
        creators["predator's swiftness and entangling roots"] = &DruidTriggerFactoryInternal::predators_swiftness_and_entangling_roots;
        creators["predator's swiftness and combat party member dead"] = &DruidTriggerFactoryInternal::predators_swiftness_and_combat_party_member_dead;
        creators["clearcasting and medium aoe"] = &DruidTriggerFactoryInternal::clearcasting_and_medium_aoe;
        creators["prowl"] = &DruidTriggerFactoryInternal::prowl_trigger;
        creators["rejuvenation blanket"] = &DruidTriggerFactoryInternal::rejuvenation_blanket;
        creators["wild growth blanket"] = &DruidTriggerFactoryInternal::wild_growth_blanket;
        creators["aquatic form"] = &DruidTriggerFactoryInternal::aquatic_form;
    }

private:
    static Trigger* natures_swiftness(PlayerbotAI* botAI) { return new NaturesSwiftnessTrigger(botAI); }
    static Trigger* natures_swiftness_active(PlayerbotAI* botAI) { return new NaturesSwiftnessActiveTrigger(botAI); }
    static Trigger* clearcasting(PlayerbotAI* botAI) { return new ClearcastingTrigger(botAI); }
    static Trigger* eclipse_solar(PlayerbotAI* botAI) { return new EclipseSolarTrigger(botAI); }
    static Trigger* eclipse_lunar(PlayerbotAI* botAI) { return new EclipseLunarTrigger(botAI); }
    static Trigger* thorns(PlayerbotAI* botAI) { return new ThornsTrigger(botAI); }
    static Trigger* thorns_on_party(PlayerbotAI* botAI) { return new ThornsOnPartyTrigger(botAI); }
    static Trigger* thorns_on_main_tank(PlayerbotAI* botAI) { return new ThornsOnMainTankTrigger(botAI); }
    static Trigger* bash(PlayerbotAI* botAI) { return new BashInterruptSpellTrigger(botAI); }
    static Trigger* faerie_fire_feral(PlayerbotAI* botAI) { return new FaerieFireFeralTrigger(botAI); }
    static Trigger* insect_swarm(PlayerbotAI* botAI) { return new InsectSwarmTrigger(botAI); }
    static Trigger* insect_swarm_on_attacker(PlayerbotAI* botAI) { return new InsectSwarmOnAttackerTrigger(botAI); }
    static Trigger* moonfire(PlayerbotAI* botAI) { return new MoonfireTrigger(botAI); }
    static Trigger* moonfire_on_attacker(PlayerbotAI* botAI) { return new MoonfireOnAttackerTrigger(botAI); }
    static Trigger* faerie_fire(PlayerbotAI* botAI) { return new FaerieFireTrigger(botAI); }
    static Trigger* natures_grasp(PlayerbotAI* botAI) { return new NaturesGraspTrigger(botAI); }
    static Trigger* berserk(PlayerbotAI* botAI) { return new BerserkTrigger(botAI); }
    static Trigger* berserk_active(PlayerbotAI* botAI) { return new BerserkActiveTrigger(botAI); }
    static Trigger* savage_roar(PlayerbotAI* botAI) { return new SavageRoarTrigger(botAI); }
    static Trigger* rake(PlayerbotAI* botAI) { return new RakeTrigger(botAI); }
    static Trigger* mark_of_the_wild(PlayerbotAI* botAI) { return new MarkOfTheWildTrigger(botAI); }
    static Trigger* mark_of_the_wild_on_party(PlayerbotAI* botAI) { return new MarkOfTheWildOnPartyTrigger(botAI); }
    static Trigger* cure_poison(PlayerbotAI* botAI) { return new CurePoisonTrigger(botAI); }
    static Trigger* party_member_cure_poison(PlayerbotAI* botAI) { return new PartyMemberCurePoisonTrigger(botAI); }
    static Trigger* entangling_roots(PlayerbotAI* botAI) { return new EntanglingRootsTrigger(botAI); }
    static Trigger* entangling_roots_kite(PlayerbotAI* botAI) { return new EntanglingRootsKiteTrigger(botAI); }
    static Trigger* hibernate(PlayerbotAI* botAI) { return new HibernateTrigger(botAI); }
    static Trigger* bear_form(PlayerbotAI* botAI) { return new BearFormTrigger(botAI); }
    static Trigger* cat_form(PlayerbotAI* botAI) { return new CatFormTrigger(botAI); }
    static Trigger* tree_form(PlayerbotAI* botAI) { return new TreeFormTrigger(botAI); }
    static Trigger* bash_on_enemy_healer(PlayerbotAI* botAI) { return new BashInterruptEnemyHealerSpellTrigger(botAI); }
    static Trigger* party_member_remove_curse(PlayerbotAI* ai) { return new DruidPartyMemberRemoveCurseTrigger(ai); }
    static Trigger* mangle_bear_trigger(PlayerbotAI* botAI) { return new MangleBearTrigger(botAI); }
    static Trigger* lacerate_trigger(PlayerbotAI* botAI) { return new LacerateTrigger(botAI); }
    static Trigger* demoralize_roar(PlayerbotAI* botAI) { return new DemoralizeRoarTrigger(botAI); }
    static Trigger* mangle_cat(PlayerbotAI* ai) { return new MangleCatTrigger(ai); }
    static Trigger* ferocious_bite_time(PlayerbotAI* ai) { return new FerociousBiteTimeTrigger(ai); }
    static Trigger* ferocious_bite_execute(PlayerbotAI* ai) { return new FerociousBiteExecuteTrigger(ai); }
    static Trigger* hurricane_channel_check(PlayerbotAI* ai) { return new HurricaneChannelCheckTrigger(ai); }
    static Trigger* no_healer_dps_strategy(PlayerbotAI* ai) { return new NoHealerDpsStrategyTrigger(ai); }
    static Trigger* starfall(PlayerbotAI* ai) { return new StarfallTrigger(ai); }
    static Trigger* force_of_nature(PlayerbotAI* ai) { return new ForceOfNatureTrigger(ai); }
    static Trigger* cyclone(PlayerbotAI* ai) { return new CycloneTrigger(ai); }
    static Trigger* predators_swiftness(PlayerbotAI* ai) { return new PredatorsSwiftnessTrigger(ai); }
    static Trigger* predators_swiftness_and_cyclone(PlayerbotAI* ai) { return new TwoTriggers(ai, "predator's swiftness", "cyclone"); }
    static Trigger* predators_swiftness_and_hibernate(PlayerbotAI* ai) { return new TwoTriggers(ai, "predator's swiftness", "hibernate"); }
    static Trigger* predators_swiftness_and_entangling_roots(PlayerbotAI* ai) { return new TwoTriggers(ai, "predator's swiftness", "entangling roots"); }
    static Trigger* predators_swiftness_and_combat_party_member_dead(PlayerbotAI* ai) { return new TwoTriggers(ai, "predator's swiftness", "combat party member dead"); }
    static Trigger* clearcasting_and_medium_aoe(PlayerbotAI* ai) { return new TwoTriggers(ai, "clearcasting", "medium aoe"); }
    static Trigger* prowl_trigger(PlayerbotAI* ai) { return new ProwlTrigger(ai); }
    static Trigger* rejuvenation_blanket(PlayerbotAI* ai) { return new BuffOnPartyTrigger(ai, "rejuvenation"); }
    static Trigger* wild_growth_blanket(PlayerbotAI* ai) { return new BuffOnPartyTrigger(ai, "wild growth"); }
    static Trigger* aquatic_form(PlayerbotAI* ai) { return new AquaticFormTrigger(ai); }
};

SharedNamedObjectContextList<Strategy> DruidAiObjectContext::sharedStrategyContexts;
SharedNamedObjectContextList<Trigger> DruidAiObjectContext::sharedTriggerContexts;
SharedNamedObjectContextList<UntypedValue> DruidAiObjectContext::sharedValueContexts;

DruidAiObjectContext::DruidAiObjectContext(PlayerbotAI* botAI)
    : AiObjectContext(botAI, sharedStrategyContexts, sharedTriggerContexts, sharedValueContexts)
{
}

void DruidAiObjectContext::BuildSharedContexts()
{
    BuildSharedStrategyContexts(sharedStrategyContexts);
    BuildSharedTriggerContexts(sharedTriggerContexts);
    BuildSharedValueContexts(sharedValueContexts);
}

void DruidAiObjectContext::BuildSharedStrategyContexts(SharedNamedObjectContextList<Strategy>& strategyContexts)
{
    AiObjectContext::BuildSharedStrategyContexts(strategyContexts);
    strategyContexts.Add(new DruidStrategyFactoryInternal());
    strategyContexts.Add(new DruidDruidStrategyFactoryInternal());
}

void DruidAiObjectContext::BuildSharedTriggerContexts(SharedNamedObjectContextList<Trigger>& triggerContexts)
{
    AiObjectContext::BuildSharedTriggerContexts(triggerContexts);
    triggerContexts.Add(new DruidTriggerFactoryInternal());
}

class DruidValueContextInternal : public NamedObjectContext<UntypedValue>
{
public:
    DruidValueContextInternal()
    {
        creators["eclipse solar proc time"] = &DruidValueContextInternal::eclipse_solar_proc_time;
        creators["eclipse lunar proc time"] = &DruidValueContextInternal::eclipse_lunar_proc_time;
    }

private:
    static UntypedValue* eclipse_solar_proc_time(PlayerbotAI* botAI) { return new EclipseSolarProcTimeValue(botAI); }
    static UntypedValue* eclipse_lunar_proc_time(PlayerbotAI* botAI) { return new EclipseLunarProcTimeValue(botAI); }
};

void DruidAiObjectContext::BuildSharedValueContexts(SharedNamedObjectContextList<UntypedValue>& valueContexts)
{
    AiObjectContext::BuildSharedValueContexts(valueContexts);
    valueContexts.Add(new DruidValueContextInternal());
}
