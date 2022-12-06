<?php

namespace App\Http\Controllers;

use App\Models\Flame;
use Illuminate\Http\Request;
use Inertia\Inertia;

class FlameController extends Controller
{
    /**
     * Shows the Flames list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Flames page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $flames = Flame::query();

        if ( $search != "" )
        {
            $flames->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $flames->orderby($field, $order);
        }

        return Inertia::render('Flames', [
            'flames' => $flames->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            
            // Send to the client an object containing the value of the search querystring
            'filters' => $request->only(['search'])
        ]);
    }
    
    /**
     * Creates a new Flame
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Flame
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Flames page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('flames')->with('error', 'Creazione fiamma non riuscita.');

        // validate() returns the validated fields on success, they can be used to create the Flame
        $validated = $request->validate([
            'name' => 'required'
        ]);

        // This is executed only if the validation succeedes
        if ( ! Flame::create($validated) )
        {
            return redirect()->route('flames')->with('error', 'Creazione fiamma non riuscita.');
        }

        return redirect()->route('flames')->with('success', 'Fiamma creata con successo.');
    }

    /**
     * Updates an Flame given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Flame
     * @param int $id id of the Flame to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Flames page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('flames')->with('error', 'Fiamma non trovata.');

        $request->validate([
            'name' => 'required'
        ]);

        $flame = Flame::find($id);

        if ( ! $flame )
        {
            return redirect()->route('flames')->with('error', 'Fiamma non trovata.');
        }

        $flame->name = $request->input('name');

        if ( ! $flame->save() )
            return redirect()->route('flames')->with('error', 'Modifica fiamma non riuscita.');
            
        return redirect()->route('flames')->with('success', 'Fiamma modificata con successo.');
    }

    /**
     * Deletes an Flame given its id
     * 
     * @param int $id id of the Flame to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Flames page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('flames')->with('error', 'Fiamma non trovata.');

        $flame = Flame::find($id);

        if ( ! $flame )
        {
            return redirect()->route('flames')->with('error', 'Fiamma non trovata.');
        }

        if ( ! $flame->delete() )
            return redirect()->route('flames')->with('error', 'Eliminazione fiamma non riuscita.');
            
        return redirect()->route('flames')->with('success', 'Fiamma eliminata con successo.');
    }
    
    /**
     * Deletes a set of Flames whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Flames to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Flames page
     */
    public function multiDelete(Request $request)
    {
        if ( Flame::destroy(collect($request->input('ids'))) )
            return redirect()->route('flames')->with('success', 'Fiamme eliminate con successo.');

        return redirect()->route('flames')->with('error', 'Eliminazione fiamme non riuscita.');
    }
}
